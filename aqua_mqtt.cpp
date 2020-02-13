#include "aqua_mqtt.h"

WiFiClient wifi_client;

AquaMqtt::AquaMqtt() {
  _status = false;
  _last_status = false;
}

void AquaMqtt::setup() {
  _mqtt_client = PubSubClient(wifi_client);
  _mqtt_client.setServer(MQTT_IP, MQTT_PORT);
  _mqtt_client.setCallback(onReceivedMessage);
}

void AquaMqtt::onReceivedMessage(char* topic, byte* payload, unsigned int length) {
  char *module = subStr(topic, "/", 2);
  char *submodule = subStr(topic, "/", 3);
  char *action = subStr(topic, "/", 4);

  StaticJsonDocument<200> data;
  DeserializationError error = deserializeJson(data, payload);
  
  if (strcmp(module,"l") == 0) { //control the lights
    aqua_lights.dispatch(submodule, action, data);
  }
}

void AquaMqtt::loop() {
  _mqtt_client.loop();
  if (!_mqtt_client.connected()) {
    _status = false;
    if (_status != _last_status) {
      if (DEBUG) Serial.println("MQTT: Disconnected");
      _last_status = false;
    }
    connect();
  }
}

void AquaMqtt::connect() {
  if (DEBUG) Serial.println("MQTT: Attempting to connect to Broker");
  
  char clientid[15];
  int randnum = random(1000,9999);
  sprintf(clientid, "%d-%s", randnum, MQTT_CLIENT);

  if (_mqtt_client.connect(clientid, MQTT_USER, MQTT_PASS)) {
    _status = _last_status = true;
    onConnect();
  }
  else {
    _status = _last_status = false;
    if (DEBUG) Serial.println("MQTT: Failed to connect to Broker");
  }
}

void AquaMqtt::onConnect() {
  if (DEBUG) Serial.println("MQTT: Connected to Broker");
  _mqtt_client.subscribe(DTOPIC);
}

bool AquaMqtt::isConnected() {
  return _status;
}

char* AquaMqtt::subStr (char* input_string, char *separator, int segment_number)
{
  char *act, *sub, *ptr;
  static char copy[100];
  int i;
 
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL)
  {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
  return sub;
}

void AquaMqtt::send(char* topic, StaticJsonDocument<200> data) 
{
  _mqtt_client.beginPublish(topic, measureJson(data), false);
  serializeJson(data, _mqtt_client);
  _mqtt_client.endPublish();
}
