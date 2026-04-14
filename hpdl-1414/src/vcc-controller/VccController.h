#ifndef VCC_CONTROLLER_h
#define VCC_CONTROLLER_h

#include "Arduino.h"

class VccController {
  public:
    VccController(uint8_t vcc_pin);
    void switch_power_on();
    void switch_power_off();
  private:
    uint8_t _vcc_pin;
};

#endif
