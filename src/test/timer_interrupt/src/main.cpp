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

#define INTERVAL 20

hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer();

IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

// deg -135 - 135
int dir1 = 1;
int dir2 = 1;
int next1, next2;
int now1, now2;

void setup() {
  Serial.begin(115200);
  krs1.begin();
  krs2.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  if (!client.connect(serverIP, serverPort)) {
    Serial.println("connection to server failed");
    return;
  }

  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, INTERVAL*1000*5, true); // 20ms
  // timerAlarmEnable(timer);

  now1 = krs1.setPos(1, 7500);
  // now2 = krs2.setPos(1, 7500);
  delay(1000);
  next1 = next2 = 7500;

  client.println("connected");
}

// deg / sec
float sp1 = 45;
float sp2 = 400;

void IRAM_ATTR onTimer() {
  // next1 = next1 + dir1 * sp1 * INTERVAL * 8 / 270;
  // next2 = next2 + dir2 * sp2 * INTERVAL * 8 / 270;

  // if (next1 > 11000 or next1 < 4000){dir1 *= -1;}
  // if (next2 > 11000 or next2 < 4000){dir2 *= -1;}
 
  // krs1.setPos(1, (int)next1);

  client.println("sending joint angle");
}

void loop() {
  client.println("hello from esp");
  delay(1000);
}
