#ifndef Persist_h
#define Persist_h

#include "Arduino.h"

// ArduinoUNO --> EEPROM	1 KB (ATmega328P)
// see https://docs.arduino.cc/learn/programming/eeprom-guide/

template <typename T>
class Persist {
  public:

    Persist(uint8_t eeprom_pin);

    // call this in the "setup" section.
    void setup();

    void save(T data);
    T read();
    bool is_valid();

  private:
    uint8_t _eeprom_pin;
    unsigned long _compute_crc(T* data);
    static const int _crc_size = sizeof(unsigned long);
};

#include "./Persist.tpp"

#endif

