#define DEBUG false

#include <PubSubClient.h>
#include <WiFiEspAT.h>
#include <Arduino.h>
#include "aqua_lights.h"
#include <ArduinoJson.h>

#define MQTT_IP "*"
#define MQTT_USER "*"
#define MQTT_PASS "*"
#define MQTT_PORT 1883
#define MQTT_CLIENT "Aquaduino"
#define DTOPIC "a1/#"

class AquaMqtt
{
  public:
    AquaMqtt::AquaMqtt();
    void setup();
    void loop();
    void connect();
    bool isConnected();
    void send(char* topic, StaticJsonDocument<200> data);
  private:
    bool _status;
    bool _last_status;
    PubSubClient _mqtt_client;
    static void onReceivedMessage(char* topic, byte* payload, unsigned int length);
    void onConnect();
    static char* subStr (char* input_string, char *separator, int segment_number);
};

extern AquaMqtt aqua_mqtt;
