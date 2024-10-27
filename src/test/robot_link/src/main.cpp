#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <math.h>
#include "IcsHardSerialClass.h"
#include "RobotLink.h"
#include "pinassign.h"

#include <WiFi.h>
#include <WiFiClient.h>

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12345;


RobotLink body;

RobotLink arm_pitch_right;
RobotLink arm_roll_right;
RobotLink hand_right;

RobotLink arm_pitch_left;
RobotLink arm_roll_left;
RobotLink hand_left;

RobotLink leg_yaw_right;
RobotLink leg_roll_right;
RobotLink leg_upper_right;
RobotLink leg_under_right;
RobotLink foot_pitch_right;
RobotLink foot_roll_right;

RobotLink leg_yaw_left;
RobotLink leg_roll_left;
RobotLink leg_upper_left;
RobotLink leg_under_left;
RobotLink foot_pitch_left;
RobotLink foot_roll_left;

#define BAUDRATE 115200
#define TIMEOUT 1000

IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

void setup_link();

void setup() {
  setup_link();
  krs1.begin();
  krs2.begin();
}

void loop() {
  // arm_pitch_right.setq(0);
  arm_pitch_right.move();
  // arm_roll_right.setq(-0.3);
  arm_roll_right.move();
  // hand_right.setq(0);
  hand_right.move();

  // arm_pitch_left.setq(0);
  arm_pitch_left.move();
  // arm_roll_left.setq(-0.3);
  arm_roll_left.move();
  // hand_left.setq(0);
  hand_left.move();

  // leg_yaw_right.setq(0);
  leg_yaw_right.move();
  // leg_roll_right.setq(0);
  leg_roll_right.move();
  // leg_upper_right.setq(-30.0 * 3.14 / 180.0);
  leg_upper_right.move();
  // leg_under_right.setq(-60.0 * 3.14 / 180.0);
  leg_under_right.move();
  // foot_pitch_right.setq(0);
  foot_pitch_right.move();
  // foot_roll_right.setq(0);
  foot_roll_right.move();

  // leg_yaw_left.setq(0);
  leg_yaw_left.move();
  // leg_roll_left.setq(0);
  leg_roll_left.move();
  // leg_upper_left.setq(-30.0 * 3.14 / 180.0);
  leg_upper_left.move();
  // leg_under_left.setq(-60.0 * 3.14 / 180.0);
  leg_under_left.move();
  // foot_pitch_left.setq(0);
  foot_pitch_left.move();
  // foot_roll_left.setq(0);
  foot_roll_left.move();

  delay(500);
}

void setup_link(){
  body.setname("body");

  arm_pitch_right.setDefault("arm_pitch_right", 0,0,64.98,  0,1,0, &body);
  arm_pitch_right.setInitialPositionsDeg(9400, 30.0 ,0,0);
  arm_pitch_right.setMotor(&krs1, 1, FORWARD);

  arm_roll_right.setDefault("arm_roll_right",   0,-62.2,0,  1,0,0, &arm_pitch_right);
  arm_roll_right.setInitialPositionsDeg(10050, -20.0 ,0,0);
  arm_roll_right.setMotor(&krs1, 2, FORWARD);

  hand_right.setDefault("hand_right",           0,0,-82.84, 0,1,0, &arm_roll_right);
  hand_right.setInitialPositionsDeg(5700, -90.0 ,0,0);
  hand_right.setMotor(&krs1, 3, REVERSE);


  
  arm_pitch_left.setDefault("arm_pitch_left", 0,0,64.98,  0,1,0, &body);
  arm_pitch_left.setInitialPositionsDeg(5930, 30,0,0);
  arm_pitch_left.setMotor(&krs1, 4, REVERSE);

  arm_roll_left.setDefault("arm_roll_left",   0, 62.2,0,  1,0,0, &arm_pitch_left);
  arm_roll_left.setInitialPositionsDeg(10300, 20,0,0);
  arm_roll_left.setMotor(&krs1, 5, REVERSE);

  hand_left.setDefault("hand_left",           0,0,-82.84, 0,1,0, &arm_roll_left);
  hand_left.setInitialPositionsDeg(6350, -90.0 ,0,0);
  hand_left.setMotor(&krs1, 6, REVERSE);


  
  leg_yaw_right.setDefault("leg_yaw_right",       0,-30,0,    0,0,1, &body);
  leg_yaw_right.setInitialPositionsDeg(7380, 0,0,0);
  leg_yaw_right.setMotor(&krs1, 7, REVERSE);

  leg_roll_right.setDefault("leg_roll_right",     0,0,-22.2,  1,0,0, &leg_yaw_right);
  leg_roll_right.setInitialPositionsDeg(9000, 0,0,0);
  leg_roll_right.setMotor(&krs2, 1, FORWARD);

  leg_upper_right.setDefault("leg_upper_right",   0,0,-26.01, 0,1,0, &leg_roll_right);
  leg_upper_right.setInitialPositionsDeg(9850, -18.31, 0,0);
  leg_upper_right.setMotor(&krs2, 2, FORWARD);

  leg_under_right.setDefault("leg_under_right",   0,0,-78.02, 0,1,0, &leg_upper_right);
  leg_under_right.setInitialPositionsDeg(4200, 65.8, 0,0);
  leg_under_right.setMotor(&krs2, 3, FORWARD);

  foot_pitch_right.setDefault("foot_pitch_right", 0,0,-78.02, 0,1,0,  &leg_under_right);
  foot_pitch_right.setInitialPositionsDeg(5890, -45.88, 0,0);
  foot_pitch_right.setMotor(&krs2, 4, REVERSE);

  foot_roll_right.setDefault("foot_roll_right",   0,0,0,      1,0,0, &foot_pitch_right);
  foot_roll_right.setInitialPositionsDeg(7600, 0,0,0);
  foot_roll_right.setMotor(&krs2, 5, REVERSE);


  
  leg_yaw_left.setDefault("leg_yaw_left",       0,30,0,     0,0,1, &body);
  leg_yaw_left.setInitialPositionsDeg(7500, 0,0,0);
  leg_yaw_left.setMotor(&krs1, 8, REVERSE);

  leg_roll_left.setDefault("leg_roll_left",     0,0,-22.2,  1,0,0, &leg_yaw_left);
  leg_roll_left.setInitialPositionsDeg(8330, 0,0,0);
  leg_roll_left.setMotor(&krs2, 6, REVERSE);

  leg_upper_left.setDefault("leg_upper_left",   0,0,-26.01, 0,1,0, &leg_roll_left);
  leg_upper_left.setInitialPositionsDeg(9750, -18.31, 0,0);
  leg_upper_left.setMotor(&krs2, 7, FORWARD);

  leg_under_left.setDefault("leg_under_left",   0,0,-78.02, 0,1,0, &leg_upper_left);
  leg_under_left.setInitialPositionsDeg(4320, 65.8, 0,0);
  leg_under_left.setMotor(&krs2, 8, FORWARD);

  foot_pitch_left.setDefault("foot_pitch_left", 0,0,-78.02, 0,1,0, &leg_under_left);
  foot_pitch_left.setInitialPositionsDeg(6090, -45.88, 0,0);
  foot_pitch_left.setMotor(&krs2, 9, REVERSE);

  foot_roll_left.setDefault("foot_roll_left",   0,0,0,      1,0,0, &foot_pitch_left);
  foot_roll_left.setInitialPositionsDeg(7400, 0,0,0);
  foot_roll_left.setMotor(&krs2, 10, REVERSE);
}
