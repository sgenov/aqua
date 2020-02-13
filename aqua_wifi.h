#define DEBUG false

#include <WiFiEspAT.h>

#define WIFI_SSID "MartinRouterKing"
#define WIFI_PASS "*"

class AquaWifi
{
  public:
    AquaWifi::AquaWifi();
    void setup();
    void printStatus();
    void loop();
    bool isConnected();
  private:
    bool _status;
    bool _last_status;
};
