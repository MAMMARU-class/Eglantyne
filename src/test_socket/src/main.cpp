#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "pinassign.h"

WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12345;

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

void loop() {
  Serial.println("program moving");
  client.println(message);
  delay(500);
}
