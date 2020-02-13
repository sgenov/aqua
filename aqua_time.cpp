#include "aqua_time.h"


void AquaTime::setup() {
  rtc.begin();
  _should_update = true;
  ntpupdate();
}

void AquaTime::loop(DateTime now) {
  int hours = now.hour();
  if (now.unixtime() - _last_update > 3600 && (_should_update || now.year() == 2000  || (hours == 3 || hours == 4) && now.minute() == 0)) {
    _should_update = true;
    ntpupdate();
  }
}

void AquaTime::ntpupdate() {
  TimeChangeRule myDST = {"EET", Last, Sun, Mar, 3, 180};
  TimeChangeRule mySTD = {"EST", Last, Sun, Oct, 4, 120};
  Timezone eetTime(myDST,mySTD);
  WiFiUDP wifi_time_client;
  EasyNTPClient ntpClient(wifi_time_client, "pool.ntp.org", 0);
  long utc = ntpClient.getUnixTime();
  wifi_time_client.stop();
  if (utc <= 0) return;
  
  long time_t = eetTime.toLocal(utc);
  rtc.adjust(time_t);
  _should_update = false;
  _last_update = time_t;
}

double AquaTime::getTemp() {
  return rtc.getTemperature();
}

DateTime AquaTime::getTodayHour(int hour, int minutes, int seconds = 0) {
  DateTime now = getNow();
  DateTime todayHour = DateTime(now.year(), now.month(), now.day(), hour, minutes, seconds);
  return todayHour;
}

DateTime AquaTime::getNow() {
  return DateTime(rtc.now());
}
