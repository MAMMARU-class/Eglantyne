#include <Arduino.h>
// micro ros libs
#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>
// msg libs
#include "std_msgs/msg/int32.h"
#include "std_msgs/msg/header.h"
#include "trajectory_msgs/msg/joint_trajectory.h"
#include "trajectory_msgs/msg/joint_trajectory_point.h"
// other needed libs
#include <vector>
#include <deque>
#include <mutex>
// my libs
#include "IcsHardSerialClass.h"
#include "Robot.h"
#include "pinassign.h"

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

// using
using std::array;
using std::vector;
using std::deque;

// control info
#define MAX_MOTION 60
// #define MAX_MOTION_SAVE 200
#define LINK_SIZE 18
#define EXP_RATIO 10 // the ratio of motion expansion
bool serial_onboard = false;
std::mutex mtx; // stop reading motion_list while publishing to motor
TaskHandle_t _spinner; // other thread
void update_servo(void *param);
// vector< vector<float> > motion_list;
deque< array<float, 20> > motion_list;

// motor serial
#define BAUDRATE 115200
#define TIMEOUT 1000
IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

// robot control object
Robot Eglantyne;

// msg macro
using JointTrajectory = trajectory_msgs__msg__JointTrajectory;
using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;
using Int32 = std_msgs__msg__Int32;
// recv
JointTrajectory trajectory_rcv;
void trajectory_rcv_init();
trajectory_msgs__msg__JointTrajectoryPoint points[MAX_MOTION];
double positions[MAX_MOTION][LINK_SIZE+2];
// trig
int motion_trigger_check;
Int32 motion_trigger;
Int32 state;

// rcl needed
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t rclc_allocator;
rcl_node_t node;
rcl_timer_t timer;

// pub sub
rcl_subscription_t traj_msg_subscriber;
rcl_publisher_t motion_trigger_pubrisher;
rcl_publisher_t state_publisher;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() { while(1) { delay(100); } }

// update motoin_list
array<float, 20> motion_get;
void update_motions(const void * msgin){

  state.data = 2;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  const JointTrajectory* msg = (const JointTrajectory *)msgin;
  int points_size = msg->points.size;

  for(int i=0; i<points_size; i++){
    // vector<float> motion;
    for(int link=0; link<msg->points.data[i].positions.size; link++){
      // motion.push_back((float)msg->points.data[i].positions.data[link]);
      motion_get[link] = (float)msg->points.data[i].positions.data[link];
    }
    motion_list.push_back(motion_get);
  }

  state.data = 3;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));
}

void setup() {
  set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
  delay(2000);

  rclc_allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &rclc_allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create subscription
  RCCHECK(rclc_subscription_init_default(
    &traj_msg_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "/motion_list_command"));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &motion_trigger_pubrisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "/motion_trigger"));

  RCCHECK(rclc_publisher_init_default(
    &state_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "/robot_state"));


  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &rclc_allocator));
  RCCHECK(rclc_executor_add_subscription(
    &executor, &traj_msg_subscriber, &trajectory_rcv, &update_motions, ON_NEW_DATA));

  trajectory_rcv_init();
  // motion_list = (vector< array<float, 20> >* )malloc(MAX_MOTION_SAVE*sizeof(vector< array<float, 20> >));

  state.data = 0;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  xTaskCreatePinnedToCore(
    update_servo, "update_servo", 
    2048, NULL, 10, &_spinner, 0 );

}

// spin node if motor doesn't move
void loop() {
  while(serial_onboard){delay(5);}
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  if(motion_trigger_check != 0){
    motion_trigger.data = (int)motion_trigger_check;
    RCSOFTCHECK(rcl_publish(&motion_trigger_pubrisher, &motion_trigger, NULL));
    motion_trigger_check = 0;
  }
  delay(5);
}

// update motor position every CONTROL_CYCLE ms
long currentMillis;
long prevMillis = 0;
void update_servo(void *param){
  // init robot
  Eglantyne.setSerial(&krs1, &krs2);
  Eglantyne.setLink();
  krs1.begin(); krs2.begin();
  // vector<float> motion_aim;
  array<float, 18> motion_ex;
  array<float, 18> motion;
  array<float, 18> motion_aim;
  array<float, 20> motion_read;
  motion_aim = Eglantyne.init_home(3);

  int count = 0;
  while(true) {
    currentMillis = millis();
    if(currentMillis - prevMillis > CONTROL_CYCLE){
      prevMillis = currentMillis;
      // if(!motion_list.empty()){
      if( !(motion_list.empty() && count == 0) ){
        // vector<float> motion;
        // mtx.lock();
        // motion_aim = motion_list.front(); motion_list.erase(motion_list.begin());
        // mtx.unlock();

        if(count == 0){
          motion_ex = motion_aim;
          mtx.lock();
          motion_read = motion_list.front();
          // motion_list.erase(motion_list.begin());
          motion_list.pop_front();
          mtx.unlock();
          for(int i=0; i<LINK_SIZE; i++){
            motion_aim[i] = motion_read[i];
          }
          // if(motion_read.size() > LINK_SIZE){
          //   motion_trigger_check = motion_read.back();
          // }
        }

        for(int i=0; i<LINK_SIZE; i++){
          motion[i] = ( motion_ex[i]*(EXP_RATIO-count) + motion_aim[i]*count ) / EXP_RATIO;
          // motion[i] = motion_aim[i];
        }
        count++;
        if(count == EXP_RATIO){
          count = 0;
        }

        // if(motion.size() > LINK_SIZE){
        //   motion_trigger_check = motion.back();
        // }
        serial_onboard = true;
        // Eglantyne.move_all(motion_aim);
        Eglantyne.move_all(motion);
        serial_onboard = false;

        delay(1);
      }else{delay(1);}
    delay(1);
    }
  }
}

// initialize message buffer
void trajectory_rcv_init(){
  // trajectory_rcv.points.data = (trajectory_msgs__msg__JointTrajectoryPoint *)malloc(MAX_MOTION*sizeof(trajectory_msgs__msg__JointTrajectoryPoint));
  trajectory_rcv.points.data = points;
  trajectory_rcv.points.size = 0;
  trajectory_rcv.points.capacity = MAX_MOTION;
  
  for(int i=0; i<MAX_MOTION; i++){
    // trajectory_rcv.points.data[i].positions.data = (double *)malloc( (LINK_SIZE+1)*sizeof(double));
    trajectory_rcv.points.data[i].positions.data = positions[i];
    trajectory_rcv.points.data[i].positions.size = 0;
    trajectory_rcv.points.data[i].positions.capacity = LINK_SIZE + 1;
  }
}
