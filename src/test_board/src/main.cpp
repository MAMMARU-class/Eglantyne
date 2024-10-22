#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "IcsHardSerialClass.h"
#include "pinassign.h"

#define BAUDRATE 115200
#define TIMEOUT 1000

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12345;


IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

void setup() {
  // Serial.begin(115200);
  krs1.begin();
  krs2.begin();
  // Serial.println("program started");

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("connected to wifi");

  // if (!client.connect(serverIP, serverPort)) {
  //   Serial.println("connection to server failed");
  //   return;
  // }

  // client.println("program started");
}

void loop() {
    // client.println("sending data");
    for(int i=4000;i<11000;i+=100){
      int now1 = krs1.setPos(1, i);
      int now2 = krs2.setPos(1, i);
      // client.printf("%d, %d", now1, now2);
      // client.println(now2);
      delay(10);
    }
    krs1.setPos(1, 4000);
    krs2.setPos(1, 4000);
    delay(1000);
    
    // //位置指令　ID:1サーボを7500へ 中央
    // int now1, now2;
    // now1 = krs1.setPos(1,7500);
    // now2 = krs2.setPos(1,7500);
    // // client.printf("%d, %d", now1, now2);
    // delay(500); //0.5秒待つ
    // now1 = krs1.setPos(1,9500); //位置指令　ID:0サーボを9500へ 右
    // now2 = krs2.setPos(1,9500); //位置指令　ID:0サーボを9500へ 右
    // // client.printf("%d, %d", now1, now2);
    // delay(500); //0.5秒待つ
    // now1 = krs1.setPos(1,7500); //位置指令　ID:0サーボを7500へ 中央
    // now2 = krs2.setPos(1,7500); //位置指令　ID:0サーボを7500へ 中央
    // // client.printf("%d, %d", now1, now2);
    // delay(500); //0.5秒待つ
    // now1 = krs1.setPos(1,5500); //位置指令　ID:0サーボを5500へ 左
    // now2 = krs2.setPos(1,5500); //位置指令　ID:0サーボを5500へ 左
    // // client.printf("%d, %d", now1, now2);
    // delay(500);
}
