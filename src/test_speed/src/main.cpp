#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "IcsHardSerialClass.h"
#include "pinassign.h"

#define BAUDRATE 115200
#define TIMEOUT 1000

#define INTERVAL 20

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "172.20.10.4";
const int serverPort = 12345;

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
  Serial.println("program started");

  now1 = krs1.setPos(1, 7500);
  now2 = krs2.setPos(1, 7500);
  delay(500);
  next1 = now1; next2 = now2;
}

static long currentMillis;
static long prevMillis = 0;

// deg / sec
float sp1 = 45;
float sp2 = 400;
void speed_control(){
  currentMillis = millis();
  if (currentMillis - prevMillis > INTERVAL){
    // now1 = krs1.getPos(1); now2 = krs2.getPos(1);
    next1 = next1 + dir1 * sp1 * INTERVAL * 8 / 270;
    next2 = next2 + dir2 * sp2 * INTERVAL * 8 / 270;

    if (next1 > 11000 or next1 < 4000){dir1 *= -1;}
    if (next2 > 11000 or next2 < 4000){dir2 *= -1;}

    now1 = krs1.setPos(1, (int)next1);
    now2 = krs2.setPos(1, (int)next2);
    prevMillis = currentMillis;
  }
}

void loop() {
  speed_control();
}
