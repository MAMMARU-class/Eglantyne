#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "pinassign.h"

#define BAUDRATE 115200
#define TIMEOUT 1000

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12344;

void setup() {
  Serial.begin(115200);

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
}

String message = "hello from esp";

static long currentMillis;
static long prevMillis = 0;
void loop() {
  currentMillis = millis();
  Serial.println("program moving");
  if( currentMillis - prevMillis > 500 ){
    client.println(message);
    client.println(currentMillis);
    prevMillis = currentMillis;
    // delay(500);
  }
}
