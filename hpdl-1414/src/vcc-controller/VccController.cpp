#include "VccController.h"
#include "Arduino.h"


VccController::VccController(uint8_t vcc_pin) {
  _vcc_pin = vcc_pin;
}

void VccController::switch_power_on() {
  pinMode(_vcc_pin, OUTPUT);
  digitalWrite(_vcc_pin, HIGH);
  delay(10); // Wait for VCC to stabilize
}

void VccController::switch_power_off() {
  digitalWrite(_vcc_pin, LOW);
  pinMode(_vcc_pin, INPUT); // High-Z: disconnect VCC safely
}