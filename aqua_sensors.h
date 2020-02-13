#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include "ArduinoHttpClient.h";
#include <WiFiEspAT.h>

#define HTTP_SERVER "*"
#define SENSORS_URIPATH "/record"
#define SECONDS_BETWEEN_REPORTS 60

#define FLOW_PIN 19

class AquaSensors
{
  public:
    void setup();
    void send(DateTime now, double temp);
    static volatile int _flow_frequency;
    static void flow();
    long getFlowRate();
  private:
    long _last_data_send;
    unsigned long _flow_current_time;
    unsigned long _flow_cloop_time;
    
};
