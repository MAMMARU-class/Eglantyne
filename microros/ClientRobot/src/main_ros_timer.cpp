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
#define MAX_MOTION 10
#define COMP_RATIO_DEFALUT 5 // the ratio of motion completion
int comp_ratio;
bool serial_onboard = false;
std::mutex mtx; // stop reading motion_list while publishing to motor
TaskHandle_t _spinner[2]; // other thread
void update_servo(void *param);
void update_rclc(void *param);
// vector< vector<float> > motion_list;
deque< array<float, LINK_SIZE+1> > motion_list;

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
double positions[MAX_MOTION][LINK_SIZE+1];
// trig
float motion_trigger_check;
Int32 motion_trigger;
Int32 state;

// rcl needed
rclc_executor_t sub_executor;
rclc_executor_t servo_executor;
rclc_support_t support;
rcl_allocator_t rclc_allocator;
rcl_node_t node;
rcl_timer_t timer;

// pub sub
rcl_subscription_t traj_msg_subscriber;
rcl_publisher_t motion_trigger_pubrisher;
rcl_publisher_t state_publisher;

// arrays for timer callback
array<float, LINK_SIZE> motion_ex;
array<float, LINK_SIZE> motion;
array<float, LINK_SIZE> motion_aim;
array<float, LINK_SIZE+1> motion_read;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() { while(1) { delay(100); } }

// subscription callback
array<float, LINK_SIZE+1> motion_get;
void update_motions(const void * msgin);

// timer callback
void update_servo(rcl_timer_t * timer, int64_t last_call_time);

// xTask
void servo_loop(void *param);
void rclc_loop(void *param);

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
    "/motion_list_command_for_esp"));

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

  // create timer
  const unsigned int timer_timeout = 50;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    update_servo));

  state.data = 0;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  // create executor
  RCCHECK(rclc_executor_init(&sub_executor, &support.context, 1, &rclc_allocator));
  RCCHECK(rclc_executor_init(&servo_executor, &support.context, 1, &rclc_allocator));
  
  RCCHECK(rclc_executor_add_subscription(
    &sub_executor, &traj_msg_subscriber, &trajectory_rcv, &update_motions, ON_NEW_DATA));
  // RCCHECK(rclc_executor_add_timer(&sub_executor, &timer));
  RCCHECK(rclc_executor_add_timer(&servo_executor, &timer));
  

  state.data = 1;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  // init msg
  trajectory_rcv_init();

  // multi tasks
  xTaskCreatePinnedToCore(
    servo_loop, "servo_loop", 
    2048, NULL, 5, &_spinner[0], 0 );
  xTaskCreatePinnedToCore(
    rclc_loop, "rclc_loop",
    2048, NULL, 1, &_spinner[1], 0 );

  state.data = 3;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  Eglantyne.setSerial(&krs1, &krs2);
  Eglantyne.setLink();
  krs1.begin(); krs2.begin();

  motion_ex = Eglantyne.current();
  // motion_aim = Eglantyne.init_home(3);
  int init_max = 3000 / (CONTROL_CYCLE*COMP_RATIO_DEFALUT);
  for(int i=0; i<init_max; i++){
    array<float, LINK_SIZE+1> init_motion;
    for(int j=0; j<LINK_SIZE; j++){
      init_motion[j] = ( motion_ex[j] * (init_max-i) + Eglantyne.home()[j] * i ) / init_max;
    }
    motion_list.push_back(init_motion);
  }
}

// timer callback
int servo_control_count = 0;
void update_servo(rcl_timer_t * timer, int64_t last_call_time){
  RCLC_UNUSED(last_call_time);
  if(timer != NULL){
    // state.data = 4;
    // RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

    if( !(motion_list.empty() && servo_control_count == 0) ){
      if(servo_control_count == 0){
        motion_ex = motion_aim;
        mtx.lock();
        motion_read = motion_list.front(); motion_list.pop_front();
        mtx.unlock();
        for(int i=0; i<LINK_SIZE; i++){
          motion_aim[i] = motion_read[i];
        }
        if(motion_read[LINK_SIZE] != 0){
          motion_trigger_check = motion_read[LINK_SIZE];
        }
      }
      for(int i=0; i<LINK_SIZE; i++){
        motion[i] = ( motion_ex[i]*(COMP_RATIO_DEFALUT-servo_control_count) + motion_aim[i]*servo_control_count ) / COMP_RATIO_DEFALUT;
      }
      servo_control_count++;
      if(servo_control_count == COMP_RATIO_DEFALUT){
        servo_control_count = 0;
      }

      serial_onboard = true;
      Eglantyne.move_all(motion);
      serial_onboard = false;
    }
  }
}

// subscription callback
void update_motions(const void * msgin){
  state.data = 2;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

  const JointTrajectory* msg = (const JointTrajectory *)msgin;
  int points_size = msg->points.size;

  for(int i=0; i<points_size; i++){
    for(int link=0; link<msg->points.data[i].positions.size; link++){
      motion_get[link] = (float)msg->points.data[i].positions.data[link];    
    }

    motion_list.push_back(motion_get);
  }

  state.data = 3;
  RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));
}

// initialize message buffer
void trajectory_rcv_init(){
  trajectory_rcv.points.data = points;
  trajectory_rcv.points.size = 0;
  trajectory_rcv.points.capacity = MAX_MOTION;
  
  for(int i=0; i<MAX_MOTION; i++){
    trajectory_rcv.points.data[i].positions.data = positions[i];
    trajectory_rcv.points.data[i].positions.size = 0;
    trajectory_rcv.points.data[i].positions.capacity = LINK_SIZE+1;
  }void loop(){
  if(motion_trigger_check != 0){
    motion_trigger.data = (int)motion_trigger_check;
    RCSOFTCHECK(rcl_publish(&motion_trigger_pubrisher, &motion_trigger, NULL));
    motion_trigger_check = 0;
  }
  // delay(5);
}

  while(true){
    RCSOFTCHECK(rclc_executor_spin_some(&servo_executor, RCL_MS_TO_NS(5)));
    delay(1);
  }
}
void loop(){
  if(motion_trigger_check != 0){
    motion_trigger.data = (int)motion_trigger_check;
    RCSOFTCHECK(rcl_publish(&motion_trigger_pubrisher, &motion_trigger, NULL));
    motion_trigger_check = 0;
  }
  // delay(5);
}
