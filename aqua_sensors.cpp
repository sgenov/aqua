#include "aqua_sensors.h"

WiFiClient wifi_sensors_client;
volatile int AquaSensors::_flow_frequency;

void AquaSensors::setup() {
  _flow_frequency = 0;
  pinMode(FLOW_PIN, INPUT);
  digitalWrite(FLOW_PIN, HIGH);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), flow, RISING);
  sei();
  _flow_current_time = millis();
  _flow_cloop_time = _flow_current_time;
  AquaSensors::_flow_frequency = 0;
}

static void AquaSensors::flow() {
  AquaSensors::_flow_frequency++;
}

long AquaSensors::getFlowRate() {
  _flow_current_time = millis();
  // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
  double seconds = (double)(_flow_current_time - _flow_cloop_time) / 1000;
  _flow_cloop_time = _flow_current_time;
  long top = (long)AquaSensors::_flow_frequency * 60;
  double bottom = 7.5 * seconds;

  long l_hour = (long)(top/bottom); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
  AquaSensors::_flow_frequency = 0; // Reset Counter
  return l_hour;
}

void AquaSensors::send(DateTime now, double temp) {
  if (now.unixtime() - _last_data_send < SECONDS_BETWEEN_REPORTS) return;

  StaticJsonDocument<200> data;
  data["l_t"] = temp;
  data["w_f"] = getFlowRate();
  
  HttpClient client = HttpClient(wifi_sensors_client, HTTP_SERVER, 80);

  client.beginRequest();
  client.post(SENSORS_URIPATH);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", measureJson(data));
  client.beginBody();
  serializeJson(data, client);
  client.endRequest();

  // read the status code and body of the response
  client.responseStatusCode();
  client.responseBody();
  
  _last_data_send = now.unixtime();
}
