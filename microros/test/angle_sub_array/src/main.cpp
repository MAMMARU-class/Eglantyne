#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rcl/error_handling.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float64_multi_array.h>

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

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Float64MultiArray multi_array_sub;
std_msgs__msg__Int32 angle_rcv;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void subscription_callback(const void * msgin){
  // const std_msgs__msg__Float64MultiArray * msg = (const std_msgs__msg__Float64MultiArray *)msgin;
  krs1.setPos(1, 6000);
  // delay(1000);
  // pos = msg->data.data[0];
  // angle_rcv.data = 100;
  // RCSOFTCHECK(rcl_publish(&publisher, &angle_rcv, NULL));
}

void setup() {
  krs1.begin(); 
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
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_node_publisher"));

  // create subscription
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64MultiArray),
    "micro_ros_array_subscriber"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &multi_array_sub, &subscription_callback, ON_NEW_DATA));
  // krs1.setPos(1, 3500);
  // delay(1000);

  multi_array_sub.data.size = 2;
  multi_array_sub.data.capacity = 5;
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
