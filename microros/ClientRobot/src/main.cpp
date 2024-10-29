#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>

#include <std_msgs/msg/string.h>
#include <trajectory_msgs/msg/joint_trajectory.h>

#include "Robot.h"
#include "pinassign.h"

// using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

#define CONTROL_CYCLE 20

#define BAUDRATE 115200
#define TIMEOUT 1000
IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

Robot Eglantyne;

rcl_subscription_t sub_motion_list;
trajectory_msgs__msg__JointTrajectory motion_list_command;
rcl_publisher_t pub_state;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t rcl_allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) { delay(100); }
}

// update motoin_list
void update_motions(const void * msgin){
  // const trajectory_msgs__msg__JointTrajectory * motion = (const trajectory_msgs__msg__JointTrajectory *)msgin;
  // std::vector< trajectory_msgs__msg__JointTrajectoryPoint > points = motion->points.data->positions;
  // for(auto& positions : points){
  //   Eglantyne.add_motion(positions);
  // }
}

void setup() {
  set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
  delay(2000);

  rcl_allocator = rcl_get_default_allocator();
  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &rcl_allocator));
  // create node
  RCCHECK(rclc_node_init_default(&node, "client_robot", "", &support));

  // create timer,
  // const unsigned int timer_timeout = 20;
  // RCCHECK(rclc_timer_init_default(
  //   &timer,
  //   &support,
  //   RCL_MS_TO_NS(timer_timeout),
  //   timer_callback));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &pub_state,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
    "/motion_trigger"));

  // create subscription
  RCCHECK(rclc_subscription_init_default(
    &sub_motion_list,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "/motion_list_command"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &rcl_allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &sub_motion_list, &motion_list_command, &update_motions, ON_NEW_DATA));

  Eglantyne.setSerial(&krs1, &krs2);
  Eglantyne.setLink();
  krs1.begin(); krs2.begin();
  Eglantyne.init_home(3);
}

long currentMillis;
long prevMillis;
int control_cycle_count;
void loop() {
  currentMillis = millis();
  if(currentMillis - prevMillis > CONTROL_CYCLE){
    prevMillis = currentMillis;


    if(control_cycle_count > 30/CONTROL_CYCLE){
      RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
      control_cycle_count = 0;
    }else{control_cycle_count++;}
  }
  delay(1);
}
