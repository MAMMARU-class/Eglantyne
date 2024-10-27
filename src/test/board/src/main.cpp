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
}

void loop() {
    int now1, now2;

    now1 = krs1.setPos(6, 7300);
    now2 = krs2.setPos(1, 3600);
    delay(500);
    now1 = krs1.setPos(6, 7800);
    now2 = krs2.setPos(1, 10000);
    delay(500);
}
