
#ifndef Blinker_h
#define Blinker_h

#include "Arduino.h"

class Blinker {
  public:
    Blinker(uint8_t led_pin);
    void update();
  private:
    uint8_t _led_pin;
    unsigned long _previous_millis;
    bool _led_state;
};

#endif