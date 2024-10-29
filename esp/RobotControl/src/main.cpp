#include "MotionController.h"
#include "pinassign.h"

#include <WiFi.h>
#include <WiFiClient.h>
WiFiClient client;
const char* ssid = "hibiki";
const char* password = "Maruh1b1k1";
const char* serverIP = "192.168.38.177";
const int serverPort = 12344;

MotionController Eglantyne;

#define BAUDRATE 115200
#define TIMEOUT 1000
IcsHardSerialClass krs1(&Serial,  MYEN1, BAUDRATE, TIMEOUT, MYRX1, MYTX1);
IcsHardSerialClass krs2(&Serial1, MYEN2, BAUDRATE, TIMEOUT, MYRX2, MYTX2);

TaskHandle_t _spinner;
void update(void *param);

void setup(){
  Eglantyne.setSerial(&krs1, &krs2);
  Eglantyne.setLink();
  krs1.begin(); krs2.begin();
  Eglantyne.init_home(3);
  // Eglantyne.setTrig(INIT_HOME);

  xTaskCreatePinnedToCore(
    update, "update", 
    2048, &Eglantyne, 10, &_spinner, 0 );

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) { delay(500); }
  // if (!client.connect(serverIP, serverPort)){ return; }
  // client.println("connected");
}

void loop(){
  while(true){
    // client.println(Eglantyne.getTrig());
    switch (Eglantyne.getTrig()){
      case STAY:
        break;
      
      case INIT_HOME:
        Eglantyne.init_home(3);
        break;
      case INIT_ZERO:
        Eglantyne.init_zero(3);
        break;

      case WALK:
        Eglantyne.oneWalkMotion(100, 60);
        break;

      default:
        break;
    }
    delay(1);
  }
}

long currentMillis;
long prevMillis = 0;
void update(void *Robot_ptr){
  MotionController* Robot = static_cast<MotionController*>(Robot_ptr);
  while(true) {
    currentMillis = millis();
    if(currentMillis - prevMillis > CONTROL_CYCLE){
      // client.println("update");
      prevMillis = currentMillis;
      std::vector<double> next_motion = (*Robot).next_motion();

      // for(double pos : next_motion){
      //   client.println(pos);
      // }

    }
    delay(1);
  }
}
