#ifndef PersistData_h
#define PersistData_h

#include "Arduino.h"
#include "../states/state-manager/TransitionCode.h"


struct Configuration {
  uint8_t prefered_luminosity;
  bool temperature_in_celsius;
  bool voltage_in_percentage;
  TransitionCode switch_on_transition;
  char switch_on_text[4];
};

#endif

