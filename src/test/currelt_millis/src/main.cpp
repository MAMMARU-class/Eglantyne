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
  Serial.begin(115200);
  // Serial1.begin(115200);
  krs1.begin();
  krs2.begin();

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
}

static long currentMillis;
static long prevMillis = 0;
int i = 4000;
int dir = 1;
int now1,now2;

void loop() {
  currentMillis = millis();
  if (currentMillis - prevMillis > 100){

    if(i == 4000){ i=5000; 
    }else{ i=4000; }
    // it wont work if used two of them at the same time 
    // now1 = krs1.setPos(1, i);
    now2 = krs2.setPos(1, i);
    prevMillis = currentMillis;
  }
}
