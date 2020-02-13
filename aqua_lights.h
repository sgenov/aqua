#pragma once
#include "aqua_time.h"
#include <ArduinoJson.h>

class AquaLights
{
  public:
    void setup();
    void loop(AquaTime aqua_time, DateTime now);
    void dispatch(char* submodule, char* action, StaticJsonDocument<200> data);
  private:
    int whiteLedValue = 0;
    int whiteLedTargetValue = 0;
    int whiteLedPins[4] = {12, 11, 10, 9};
    bool imposedWhiteLed = 0;
    void changeAllPins(int pins[], int value);
    void defaultBehaviorToggle(StaticJsonDocument<200> data);
    void setCustomWhiteIntensity(int data);
};

extern AquaLights aqua_lights;
