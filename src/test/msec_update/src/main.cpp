#include <Arduino.h>
#include <iostream>
#include <vector>
#include <WiFi.h>
#include <WiFiClient.h>
#include "IcsHardSerialClass.h"
#include "pinassign.h"

#include <WiFi.h>
#include <WiFiClient.h>

#define INTERVAL 20

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12344;

#define BAUDRATE 115200
#define TIMEOUT 1000

IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

TaskHandle_t _task_1;
void motor1_update(void *param);
// TaskHandle_t _task_2;
// void motor2_update(void *param);

struct MotionData{
  int aim;
  std::vector<int> motor1_motion = {};
  std::vector<int> motor2_motion = {};
};
MotionData motion_data;

std::vector<int> pos_list = {750007500, 750007500};
void setup() {
  krs1.begin();
  krs2.begin();


  xTaskCreatePinnedToCore(
    motor1_update, // pointor to the function
    "motor1_update", // task name
    2048, // stack size
    &pos_list, // task param pointor
    10, // priority(0-25)
    &_task_1, // task handler
    0 // cpu num. c3 has 0 only
    );
}


int dir1 = 1;
int sp1 = 100;
int m1current = 7500;

int dir2 = 1;
int sp2 = 500;
int m2current = 7500;
void loop() {
  while(true){
    sp1 = 100;
    sp2 = 500;
    for(int i=0; i<100; i++){
      m1current += sp1*dir1;
      if( m1current > 10000 or m1current < 4000){dir1 *= -1;}
      m2current += sp2*dir2;
      if( m2current > 10000 or m2current < 4000){dir2 *= -1;}

      pos_list.push_back(m1current * 10000 + m2current);
    }
    delay(5000);
    sp1 = 500;
    sp2 = 1000;
    for(int i=0; i<100; i++){
      m1current += sp1*dir1;
      if( m1current > 10000 or m1current < 4000){dir1 *= -1;}
      m2current += sp2*dir2;
      if( m2current > 10000 or m2current < 4000){dir2 *= -1;}

      pos_list.push_back(m1current * 10000 + m2current);
    }
    delay(5000);
  }
}

/*
マルチタスクの中で参照できないものを参照するとバグる
二重リストや構造体も参照できない。

よって関節角度を1重のint型リストに変換して送り、中でデコードするしかない。
また参照できないものを参照することを防ぐために必ずリストの中の最後の値は消さないようにする。
また、.size()や.empty()も使えないらしいので、イテレータの引き算からリストの長さを計算することにした。
*/
int aim = 4000;
long currentMillis_1;
long prevMillis_1 = 0;
void motor1_update(void *motion_data) {
  std::vector<int> *data = static_cast<std::vector<int>*>(motion_data);
  int prev_aim1, prev_aim2;
  while(true) {
    currentMillis_1 = millis();
    if(currentMillis_1 - prevMillis_1 > INTERVAL){
      int vector_size = (*data).end() - (*data).begin();
      if( vector_size > 1){
        prevMillis_1 = currentMillis_1;
        int data_list = (*(*data).begin());
        data->erase((*data).begin());
        int aim1 = data_list/10000;
        int aim2 = data_list - aim1*10000;
        krs1.setPos(1, aim1);
        krs2.setPos(1, aim2);
        prev_aim1 = aim1; prev_aim2 = aim2;
      }else{
        krs1.setPos(1, prev_aim1);
        krs2.setPos(1, prev_aim2);
        prevMillis_1 = currentMillis_1;
      }
    }
  }
}
