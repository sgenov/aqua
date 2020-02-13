#pragma once
#include <EasyNTPClient.h>
#include <WiFiEspAT.h>
#include <RTClib.h>
#include <Timezone.h> 

class AquaTime
{
  public:
    void setup();
    double getTemp();
    DateTime getTodayHour(int hour, int minutes, int seconds = 0);
    DateTime getNow();
    void loop(DateTime now);
  private:
    RTC_DS3231 rtc;
    void ntpupdate();
    bool _should_update = false;
    long _last_update = 0; 
};
