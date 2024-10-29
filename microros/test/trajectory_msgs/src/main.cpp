#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <trajectory_msgs/msg/joint_trajectory.h>

#include "IcsHardSerialClass.h"
#include "pinassign.h"

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

#define CONTROL_CYCLE 20

// #define BAUDRATE 115200
// #define TIMEOUT 1000
// IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
// IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

using JointTrajectory = trajectory_msgs__msg__JointTrajectory;
using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;
using Int32 = std_msgs__msg__Int32;
JointTrajectory msg_rcv;
JointTrajectory trajectory_rcv;
JointTrajectoryPoint trajectorypoint;
Int32 rcv_check;
// int pos = 7500;

rcl_publisher_t publisher;
rcl_publisher_t pub_rcv_check;
rcl_subscription_t subscriber;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void subscription_callback(const void * msgin){
  rcv_check.data = 0;
  RCSOFTCHECK(rcl_publish(&pub_rcv_check, &rcv_check, NULL));

  const JointTrajectory * msg = (const JointTrajectory *)msgin;
  msg_rcv.points = msg->points;

  RCSOFTCHECK(rcl_publish(&publisher, &msg_rcv, NULL));
  trajectorypoint = (*msg->points.data);
}

void setup() {
  // krs1.begin(); 
  // krs2.begin();

  set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "micro_ros_node_publisher"));
    // create publisher
  RCCHECK(rclc_publisher_init_default(
    &pub_rcv_check,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "rcv_check"));
  // create subscription
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "/motion_list_command"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &trajectory_rcv, &subscription_callback, ON_NEW_DATA));
}

long currentMillis;
long prevMillis;
int count;
void loop() {
  currentMillis = millis();
  if(currentMillis - prevMillis > CONTROL_CYCLE){
    prevMillis = currentMillis;
    // for(int id=1; id<=8; id++){
    // krs1.setPos(id, trajectorypoint.positions.data[id-1]);
    // }
    // for(int id=1; id<=10; id++){
    // krs2.setPos(id, trajectorypoint.positions.data[id-1+8]);
    // }
    if(count > 30/CONTROL_CYCLE){
      RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
      count = 0;
    }else{count++;}
  }
  delay(1);
}
