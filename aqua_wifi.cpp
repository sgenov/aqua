#include "aqua_wifi.h"

AquaWifi::AquaWifi() {
  _status = false;
  _last_status = false;
}

void AquaWifi::setup()
{
  WiFi.init(Serial3);
  WiFi.endAP(true);
  WiFi.setPersistent();

  if (DEBUG) {
    Serial.println();
    Serial.print("WIFI: Attemting to connect to SSID: ");
    Serial.println(WIFI_SSID);
  }

  _status = _last_status = WiFi.begin(WIFI_SSID, WIFI_PASS);

  if (_status == WL_CONNECTED) {
    WiFi.setAutoConnect(true);
    if (DEBUG) {
      Serial.println();
      Serial.println("WIFI: Connected to network.");
      printStatus();
    }
  }
  else {
    WiFi.disconnect();
    if (DEBUG) {
      Serial.println();
      Serial.println("WIFI: Connection failed.");
    }
  }
}

void AquaWifi::printStatus()
{
  // print the SSID of the network you're attached to:
  char ssid[33];
  WiFi.SSID(ssid);
  Serial.print("SSID: ");
  Serial.println(ssid);

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

bool AquaWifi::isConnected()
{
  return _status;  
}

void AquaWifi::loop() {
  _status = WiFi.status() == WL_CONNECTED;
  if (_status != _last_status) {
    if (_status) {
      Serial.println("WiFi: Connected");
    }
    else {
      Serial.println("WiFi: Disconnected");
    }
    _last_status = _status;
  }
}
