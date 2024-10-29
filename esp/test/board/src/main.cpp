#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "IcsHardSerialClass.h"
#include "pinassign.h"

#define BAUDRATE 115200
#define TIMEOUT 1000

IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

void setup() {
  krs1.begin();
  krs2.begin();

  krs1.setPos(1, 7500);
  krs2.setPos(1, 7500);
}

void loop() {
    krs1.setPos(1, 7300);
    krs2.setPos(1, 3600);
    delay(500);
    krs1.setPos(1, 7800);
    krs2.setPos(1, 10000);
    delay(500);
}
