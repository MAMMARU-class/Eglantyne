#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>

#include "std_msgs/msg/int32.h"
#include "std_msgs/msg/header.h"
#include "trajectory_msgs/msg/joint_trajectory.h"
#include "trajectory_msgs/msg/joint_trajectory_point.h"

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

using JointTrajectory = trajectory_msgs__msg__JointTrajectory;
using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;
using JointTrajectoryPointSequence = trajectory_msgs__msg__JointTrajectoryPoint__Sequence;

rcl_publisher_t publisher;
JointTrajectory trajectory;

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

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher, &trajectory, NULL));
  }
}

void setup() {
  set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "micro_ros_joint_trajectory_publisher"));

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));


  // setting of trajectory_msgs/msg/JointTrajectory
  // initialize points
  trajectory.points.size = 5;
  trajectory.points.capacity = 8;
  trajectory.points.data = (JointTrajectoryPoint *)malloc(trajectory.points.capacity * sizeof(JointTrajectoryPoint));
  // add position data to points
  for(size_t i=0; i<trajectory.points.size; i++){
    trajectory_msgs__msg__JointTrajectoryPoint__init(&trajectory.points.data[i]);

    trajectory.points.data[i].positions.size = 5;
    trajectory.points.data[i].positions.capacity = 8;
    trajectory.points.data[i].positions.data = (double *)malloc(trajectory.points.data[i].positions.capacity * sizeof(double));

    for(size_t j=0; j<trajectory.points.data[i].positions.size; ++j){
      trajectory.points.data[i].positions.data[j] = i*10.0 + j;
    }
  }
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
