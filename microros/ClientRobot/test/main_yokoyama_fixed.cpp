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
using std::deque;
using std::vector;

// control info
constexpr unsigned int MAX_MOTION = 60;
constexpr unsigned int COMP_RATIO_DEFALUT = 5; // the ratio of motion completion
int comp_ratio;
bool serial_onboard = false;
std::mutex mtx;           // stop reading motion_list while publishing to motor
TaskHandle_t _spinner[2]; // other thread
void update_servo(void *param);
void update_rclc(void *param);
// vector< vector<float> > motion_list;
deque<array<float, LINK_SIZE + 1>> motion_list;

// motor serial
constexpr unsigned int BAUDRATE = 115200;
constexpr unsigned int TIMEOUT = 1000;
IcsHardSerialClass krs1(&Serial, MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
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
double positions[MAX_MOTION][LINK_SIZE + 1];
// trig
float motion_trigger_check;
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

#define RCCHECK(fn)                  \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
            error_loop();            \
        }                            \
    }
#define RCSOFTCHECK(fn)              \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
        }                            \
    }

// Error handle loop
void error_loop()
{
    while (1)
    {
        delay(100);
    }
}

// update motoin_list
array<float, LINK_SIZE + 1> motion_get;
void update_motions(const void *msgin)
{

    state.data = 2;
    RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

    const JointTrajectory *msg = (const JointTrajectory *)msgin;
    int points_size = msg->points.size;

    for (int i = 0; i < points_size; i++)
    {
        // vector<float> motion;
        for (int link = 0; link < msg->points.data[i].positions.size; link++)
        {
            motion_get[link] = (float)msg->points.data[i].positions.data[link];
        }

        motion_list.push_back(motion_get);
    }

    state.data = 3;
    RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));
}

void setup()
{
    set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
    delay(2000);

    rclc_allocator = rcl_get_default_allocator();
    // create init_options
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

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &rclc_allocator));
    RCCHECK(rclc_executor_add_subscription(
        &executor, &traj_msg_subscriber, &trajectory_rcv, &update_motions, ON_NEW_DATA));

    trajectory_rcv_init();

    state.data = 0;
    RCSOFTCHECK(rcl_publish(&state_publisher, &state, NULL));

    xTaskCreatePinnedToCore(
        update_servo, "update_servo",
        2048, NULL, 10, &_spinner[0], 0);

    xTaskCreatePinnedToCore(
        update_rclc, "update_rclc",
        2048, NULL, 1, &_spinner[1], 0);
}

void loop(){}

void update_rclc(void *param)
{
    while (true)
    {
        if (mtx.try_lock())
        {
            RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1)));
            // if (motion_trigger_check != 0)
            // {
            //     motion_trigger.data = (int)motion_trigger_check;
            //     RCSOFTCHECK(rcl_publish(&motion_trigger_pubrisher, &motion_trigger, NULL));
            //     motion_trigger_check = 0;
            // }
            mtx.unlock();
        }
    }
}

// // update motor position every CONTROL_CYCLE ms
// void update_servo(void *param)
// {
//     // init robot
//     Eglantyne.setSerial(&krs1, &krs2);
//     Eglantyne.setLink();
//     krs1.begin();
//     krs2.begin();
//     // vector<float> motion_aim;
//     array<float, LINK_SIZE> motion;
//     array<float, LINK_SIZE> motion_now = Eglantyne.current();
//     array<float, LINK_SIZE> motion_aim;
//     array<float, LINK_SIZE + 1> motion_read;
//     motion_aim = Eglantyne.init_home(3);

//     int count = COMP_RATIO_DEFALUT;
//     long millis_pre = millis();
//     while (true)
//     {
//         if (count == COMP_RATIO_DEFALUT){
//             if (motion_list.empty())
//             { count = 1; }
//             else
//             {
//                 motion_read = motion_list.front();
//                 motion_list.pop_front();
//                 std::copy(motion_read.begin(), motion_read.end(), std::begin(motion_aim));

//                 if (motion_read[LINK_SIZE] != 0)
//                 {
//                     motion_trigger_check = motion_read[LINK_SIZE];
//                 }
//             }
//         }

//         for (int i = 0; i < LINK_SIZE; i++)
//         {
//             auto motion_diff = motion_aim[i] - motion_now[i];
//             motion[i] = (motion_now[i] + motion_diff * count / COMP_RATIO_DEFALUT);
//         }
//         // mtx.lock();
//             Eglantyne.move_all(motion);
//         // mtx.unlock();
//             count--;

//         if (count = 0)
//         {
//             std::copy(motion_aim.begin(), motion_aim.end(), std::begin(motion_now));
//             count = COMP_RATIO_DEFALUT;
//         }

//         long mills_diff = millis() - millis_pre;
//         if (mills_diff < CONTROL_CYCLE)
//         {
//             delay(CONTROL_CYCLE - mills_diff);
//         }
//         millis_pre = millis();
//     }
// }

// update motor position every CONTROL_CYCLE ms
long currentMillis;
long prevMillis = 0;
void update_servo(void *param){
  // vTaskSuspend(_rclc_spinner);
  // init robot
  Eglantyne.setSerial(&krs1, &krs2);
  Eglantyne.setLink();
  krs1.begin(); krs2.begin();
  // vector<float> motion_aim;
  array<float, LINK_SIZE> motion_ex = Eglantyne.current();
  array<float, LINK_SIZE> motion;
  array<float, LINK_SIZE> motion_aim;
  array<float, LINK_SIZE+1> motion_read;
  motion_aim = Eglantyne.init_home(3);

  int count = 0;
  while(true) {
    currentMillis = millis();
    if(currentMillis - prevMillis > CONTROL_CYCLE){
      prevMillis = currentMillis;
      // if(!motion_list.empty()){
      if( !(motion_list.empty() && count == 0) ){
        if(count == 0){
          motion_ex = motion_aim;
          motion_read = motion_list.front(); motion_list.pop_front();
          for(int i=0; i<LINK_SIZE; i++){
            motion_aim[i] = motion_read[i];
          }
          if(motion_read[LINK_SIZE] != 0){
            motion_trigger_check = motion_read[LINK_SIZE];
          }
        }

        for(int i=0; i<LINK_SIZE; i++){
          motion[i] = ( motion_ex[i]*(COMP_RATIO_DEFALUT-count) + motion_aim[i]*count ) / COMP_RATIO_DEFALUT;
        }
        count++;
        // if(count >= COMP_RATIO_DEFALUT*comp_ratio){
        if(count == COMP_RATIO_DEFALUT){
          count = 0;
        }

        // if(motion.size() > LINK_SIZE){
        //   motion_trigger_check = motion.back();
        // }
        // serial_onboard = true;
        mtx.lock();
        Eglantyne.move_all(motion);
        mtx.unlock();
        // serial_onboard = false;

        delay(1);
      }else{delay(1);}
    delay(1);
    // vTaskResume(_rclc_spinner);
    }
  }
}

// initialize message buffer
void trajectory_rcv_init()
{
    trajectory_rcv.points.data = points;
    trajectory_rcv.points.size = 0;
    trajectory_rcv.points.capacity = MAX_MOTION;

    for (int i = 0; i < MAX_MOTION; i++)
    {
        trajectory_rcv.points.data[i].positions.data = positions[i];
        trajectory_rcv.points.data[i].positions.size = 0;
        trajectory_rcv.points.data[i].positions.capacity = LINK_SIZE + 1;
    }
}
