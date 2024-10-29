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

#include "IcsHardSerialClass.h"
#include "pinassign.h"

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

#define CONTROL_CYCLE 20

#define BAUDRATE 115200
#define TIMEOUT 1000
IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
// IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);
int pos = 7500;

using JointTrajectory = trajectory_msgs__msg__JointTrajectory;
using JointTrajectoryPoint = trajectory_msgs__msg__JointTrajectoryPoint;
JointTrajectory trajectory_rcv;
JointTrajectory trajectory_pub;

rcl_subscription_t subscriber;
rcl_publisher_t publisher;

rcl_publisher_t rcv_check;
std_msgs__msg__Int32 rcv_check_msg;

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
  // const JointTrajectory* msg = (const JointTrajectory *)msgin;
  pos = 3500;
  // RCSOFTCHECK(rcl_publish(&rcv_check, &rcv_check_msg, NULL));

  // copy received message to trajectory_pub
  // set headder
  // trajectory_pub.header.frame_id.data = (char *)malloc(10 * sizeof(char));
  // strcpy(trajectory_pub.header.frame_id.data, "traj_rcv");
  // trajectory_pub.header.frame_id.size = strlen(trajectory_pub.header.frame_id.data);
  // trajectory_pub.header.frame_id.capacity = 10;
  // // initialize points (list)
  // trajectory_pub.points.size = msg->points.size;
  // trajectory_pub.points.capacity = msg->points.capacity;
  // trajectory_pub.points.data = (JointTrajectoryPoint *)malloc(trajectory_pub.points.capacity * sizeof(JointTrajectoryPoint));
  // // add position data to points
  // for(size_t i=0; i<trajectory_pub.points.size; i++){
  //   trajectory_msgs__msg__JointTrajectoryPoint__init(&trajectory_pub.points.data[i]);

  //   trajectory_pub.points.data[i].positions.size = msg->points.data[i].positions.size;
  //   trajectory_pub.points.data[i].positions.capacity = msg->points.data[i].positions.size;
  //   trajectory_pub.points.data[i].positions.data = (double *)malloc(trajectory_pub.points.data[i].positions.capacity * sizeof(double));

  //   for(size_t j=0; j<trajectory_pub.points.data[i].positions.size; ++j){
  //     trajectory_pub.points.data[i].positions.data[j] = msg->points.data[i].positions.data[j];
  //   }
  // }

  // RCSOFTCHECK(rcl_publish(&publisher, &trajectory_pub, NULL));
}

void setup() {
  krs1.begin(); 
  set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create subscription
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "/motion_list_command"));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(trajectory_msgs, msg, JointTrajectory),
    "/motion_list_command_sub"));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &rcv_check,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "/rcv_check"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(
    &executor, &subscriber, &trajectory_rcv, &subscription_callback, ON_NEW_DATA));
  
  rcv_check_msg.data = 0;
}

long currentMillis;
long prevMillis;
int count;
void loop() {
  currentMillis = millis();
  if(currentMillis - prevMillis > CONTROL_CYCLE){
    prevMillis = currentMillis;
    krs1.setPos(1, pos);
    // krs2.setPos(1, pos);
    if(count > 100/CONTROL_CYCLE){
      RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
      count = 0;
    }else{count++;}
  }
  delay(1);
}
