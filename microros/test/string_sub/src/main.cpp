#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/string.h>

#if !defined(MICRO_ROS_TRANSPORT_ARDUINO_SERIAL)
#error This example is only avaliable for Arduino framework with serial transport.
#endif

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;

rcl_publisher_t pub_string;
rcl_subscription_t sub_string;

std_msgs__msg__String send_string_msg;
std_msgs__msg__String recv_string_msg;
#define STR_SIZE (100) //最大の受信文字数

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}


// Callback
void callback_string(const void *raw_msg){
  	std_msgs__msg__String *msg = (std_msgs__msg__String *)raw_msg;

	String s = "echo : " + String(msg->data.data);

	char strBuf[120]; s.toCharArray(strBuf, 120);
	send_string_msg.data.size = s.length();
	send_string_msg.data.data = strBuf;

	// 配信
	// rcl_publish(&pub_string, &send_string_msg, NULL);
  RCSOFTCHECK(rcl_publish(&pub_string, &send_string_msg, NULL));
}

void setup(){
	// 通信の初期化
	// Wi-Fi経由の場合
	set_microros_wifi_transports("hibiki", "Maruh1b1k1", "192.168.38.177", 8888);
	// USB経由の場合
	// set_microros_transports();

	// 初期化完了までの待機時間
	delay(2000);
	
	// micro-ROSのためのメモリ管理
	allocator = rcl_get_default_allocator();

	// micro-ROSのためのサポートクラス
	RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  //create init_options
  // RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_node", "", &support));

	// Publisher 作成
	// rmw_qos_profile_****を変更することで、qosを変更できる（カスタムも可能）
	// rclc_publisher_init(&pub_string, &node, 
	// 					ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
	// 					"echo", &rmw_qos_profile_default
	// );

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &pub_string,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
    "echo"));
  
  RCCHECK(rclc_subscription_init_default(
    &sub_string,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
    "recv"));

	// // Subscriber 作成
	// rclc_subscription_init(&sub_string, &node,
	// 						ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
	// 						"recv", &rmw_qos_profile_default
	// );
	
	// コールバックを管理ためのexecutor
	// Subscriber、Timer、Serviceなどもコールバック関数を設定する
	// Publisherだけなら、以降の処理は必要ない
	
	// int callback_size = 1;	// コールバックを行う数
	// executor = rclc_executor_get_zero_initialized_executor();
	// rclc_executor_init(&executor, &support.context, callback_size, &allocator);
	// rclc_executor_add_subscription(&executor, &sub_string, &recv_string_msg, 
	// 							   &callback_string, ON_NEW_DATA);
  
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &sub_string, &recv_string_msg, &callback_string, ON_NEW_DATA));
  
	// char型変数を入れる配列を確保する
	recv_string_msg.data.data = (char * )malloc(STR_SIZE * sizeof(char));
	recv_string_msg.data.size = 0;
	recv_string_msg.data.capacity = STR_SIZE;
}

void loop(){
	// 受信
  	rclc_executor_spin_some(&executor, RCL_MS_TO_NS(200));
  	delay(10);
}
