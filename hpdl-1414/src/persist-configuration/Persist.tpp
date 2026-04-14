
#include "EEPROM.h"
#include "Arduino.h"
#include "./Persist.h"


// eeprom_pin should be the LED pin, aka pin 13.
template <typename T>
Persist<T>::Persist(uint8_t eeprom_pin) {
  _eeprom_pin = eeprom_pin;
}

template <typename T>
void Persist<T>::setup() {
  pinMode(_eeprom_pin, OUTPUT);
}

template <typename T>
void Persist<T>::save(T data) {
  uint32_t crc = _compute_crc(&data);
  EEPROM.put(_eeprom_pin, crc);
  EEPROM.put(_eeprom_pin + _crc_size, data);
}

template <typename T>
T Persist<T>::read() {
  uint32_t stored_crc;
  T data;
  EEPROM.get(_eeprom_pin, stored_crc);
  EEPROM.get(_eeprom_pin + _crc_size, data);
  return stored_crc == _compute_crc(&data) ? data : T();
}

template <typename T>
bool Persist<T>::is_valid() {
  uint32_t stored_crc;
  T data;
  EEPROM.get(_eeprom_pin, stored_crc);
  EEPROM.get(_eeprom_pin + _crc_size, data);
  return stored_crc == _compute_crc(&data);
}

template <typename T>
unsigned long Persist<T>::_compute_crc(T* data) {
  const uint32_t crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };
  uint32_t crc = ~0L;
  const uint8_t* byte_data = reinterpret_cast<const uint8_t*>(data);

  for (int i = 0; i < sizeof(T); ++i) {
    crc = crc_table[(crc ^ byte_data[i]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (byte_data[i] >> 4)) & 0x0f] ^ (crc >> 4);
  }
  return ~crc;
}
