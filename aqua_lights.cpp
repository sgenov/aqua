#include "aqua_lights.h"
#include "Arduino.h"
#include "aqua_mqtt.h"

void AquaLights::setup() {
  
  for (int num = 0; num <= 4; num++) {
    pinMode(whiteLedPins[num],OUTPUT);
    analogWrite(whiteLedPins[num],whiteLedValue);
  }
}

void AquaLights::loop(AquaTime aqua_time, DateTime now) {
  if (!imposedWhiteLed) {
    if (whiteLedValue > 0) { //If turned on
      DateTime end_time = aqua_time.getTodayHour(16,30);
      DateTime start_time = aqua_time.getTodayHour(9,30);
      if (now > end_time || now < start_time) { //And we go past our bedtime
        whiteLedTargetValue = 0;
      }
    }
    if (whiteLedValue < 255) { //If off or not fully on
      DateTime end_time = aqua_time.getTodayHour(16,30);
      DateTime start_time = aqua_time.getTodayHour(9,30);
      if (now < end_time && now > start_time) { //And we are in the middle of the day
        whiteLedTargetValue = 255;
      }
    }
  }
  bool change_needed = false;
  if (whiteLedValue > whiteLedTargetValue) {
    whiteLedValue--;
    change_needed = true;
  }
  else if (whiteLedValue < whiteLedTargetValue) {
    whiteLedValue++;
    change_needed = true;
  }

  if (change_needed) {
    changeAllPins(whiteLedPins,whiteLedValue);

    StaticJsonDocument<200> data;
    data["v"] = whiteLedValue;
    aqua_mqtt.send("d/l/w", data);
  }
}

void AquaLights::changeAllPins(int pins[], int value) {
  for (int num = 0; num < 4; num++) {
    analogWrite(pins[num],value);
  }
}

void AquaLights::dispatch(char* submodule, char* action, StaticJsonDocument<200> data) {
  if (strcmp(submodule,"w") == 0) {
    if (strcmp(action,"ci") == 0) {
      setCustomWhiteIntensity((int) data["v"]);
    }
  }
  if (strcmp(submodule,"a") == 0) {
    if (strcmp(action,"r") == 0) {
      defaultBehaviorToggle(data);
    }
  }
}

void AquaLights::setCustomWhiteIntensity(int data) {
  imposedWhiteLed = true;
  whiteLedTargetValue = data;
  StaticJsonDocument<200> dataJSON;
  dataJSON["v"] = 0;
  aqua_mqtt.send("d/l/a/r", dataJSON);
}

void AquaLights::defaultBehaviorToggle(StaticJsonDocument<200> data) {
  imposedWhiteLed = !(bool) data["v"];
  aqua_mqtt.send("d/l/a/r", data);
}
