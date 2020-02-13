#define AT_BAUD_RATE 115200
#define DEBUG false

#include "Arduino.h"
#include "aqua_wifi.h"
#include "aqua_mqtt.h"
#include "aqua_sensors.h"

AquaWifi aqua_wifi;
AquaMqtt aqua_mqtt;
AquaTime aqua_time;
AquaLights aqua_lights;
AquaSensors aqua_sensors;

void setup() {
  Serial.begin(AT_BAUD_RATE);
  Serial3.begin(AT_BAUD_RATE);
  aqua_wifi.setup();
  if (aqua_wifi.isConnected()) {
    aqua_mqtt.setup();
    aqua_mqtt.connect();
    if (aqua_mqtt.isConnected()) {
      aqua_time.setup();
    }
  }
  aqua_lights.setup();
  aqua_sensors.setup();
}

void loop() {
  delay(200);
  aqua_wifi.loop();
  aqua_mqtt.loop();
  DateTime now = aqua_time.getNow();
  
  aqua_lights.loop(aqua_time, now);

  if (aqua_wifi.isConnected()) {
    aqua_time.loop(now);
    if (aqua_mqtt.isConnected()) {
      //Serial.println("Mqtt is connected !");
    }
    else {
      //Serial.println("Mqtt is not connected !");
    }
    aqua_sensors.send(now,aqua_time.getTemp());
  }
  else {
    //Serial.println("We are not connected !");
  }
}
