#include "Arduino.h"
#include "Blinker.h"

Blinker::Blinker(uint8_t led_pin) {
  _led_pin = led_pin;
  _previous_millis = -1;
  _led_state = false;
  pinMode(_led_pin, OUTPUT);
  digitalWrite(_led_pin, _led_state);
}

void Blinker::update() {
  uint32_t current_millis = millis();
  uint16_t delay = _led_state ? 1 : 2000;
  if (current_millis - _previous_millis >= delay) {
    _previous_millis = current_millis;
    _led_state = !_led_state;
    digitalWrite(_led_pin, _led_state);
  }
}