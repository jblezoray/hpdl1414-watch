#include "Arduino.h"
#include "Hpdl1414Writer.h"

Hpdl1414Writer::Hpdl1414Writer(
  const uint8_t data_pins[],
  const uint8_t address_pins[],
  const uint8_t write_pin
) {
  _refresh_screen_counter = 0;

  _fade_in_out_time_ms = 150;
  _fade_in_out_start_time_ms = 0;
  _fade_in = false;
  _fade_out = false;
  _fade_in_out_chars = 0b1111;
  memset(_fade_max_luminosity, MAX_LUMINOSITY, 4 * sizeof(uint8_t));

  _blink_time_on_ms = 0;
  _blink_time_off_ms = 0;
  _blink_start_time_ms = 0;
  _blinking_chars = 0x00;

  memset(_screen_chars, ' ', 4 * sizeof(char));
  memset(_screen_written, ' ', 4 * sizeof(char));
  memset(_previous_screen_written, ' ', 4 * sizeof(char));

  set_luminosity(MAX_LUMINOSITY);

  for (uint8_t i=0; i<DATA_PINS_SIZE; i++)
    _data_pins[i] = data_pins[i];
  for (uint8_t i=0; i<ADDRESS_PINS_SIZE; i++)
    _address_pins[i] = address_pins[i];
  _write_pin = write_pin;
}


void Hpdl1414Writer::switch_on() {
  for (uint8_t i=0; i<DATA_PINS_SIZE; i++) {
    pinMode(_data_pins[i], OUTPUT);
  }
  for (uint8_t i=0; i<ADDRESS_PINS_SIZE; i++) {
    pinMode(_address_pins[i], OUTPUT);
  }
  pinMode(_write_pin, OUTPUT);
}

void Hpdl1414Writer::switch_off() {
  // High-Z: disconnect all pins safely
  for (uint8_t i=0; i<DATA_PINS_SIZE; i++) {
    pinMode(_data_pins[i], INPUT);
  }
  for (uint8_t i=0; i<ADDRESS_PINS_SIZE; i++) {
    pinMode(_address_pins[i], INPUT);
  }
  // INPUT_PULLUP to keep it HIGH (=not in writing state)
  pinMode(_write_pin, INPUT);
}

void Hpdl1414Writer::setup(int default_luminosity) {
  set_luminosity(default_luminosity);
  switch_on();
  _write_screen(true);
}

void Hpdl1414Writer::refresh_screen() {
  _refresh_screen_counter++;

  // luminosity management
  _update_luminosity_on_fade_in_out();
  for (uint8_t i = 0; i<4; i++) {
    bool show = _screen_luminosity_pattern[i] >> (_refresh_screen_counter % 16) & 0x01 != 0;
    _screen_written[i] = show ? _screen_chars[i] : ' ';
  }

  // blinking management
  if (_blink_start_time_ms>0) {
    uint16_t blink_period = _blink_time_on_ms + _blink_time_off_ms;
    bool is_on = (millis() - _blink_start_time_ms) % blink_period < _blink_time_on_ms;
    if (!is_on) {
      for (uint8_t i = 0; i<4; i++) {
        if ((_blinking_chars >> (3-i)) & 0x1) {
          _screen_written[i] = ' ';
        }
      }
    }
  }

  // write
  _write_screen(false);
}

void Hpdl1414Writer::_write_screen(bool force) {
  for (uint8_t position = 0; position<4; position++) {
    // cache writes.
    if (!force && _previous_screen_written[position] == _screen_written[position]) {
      continue;
    }
    _previous_screen_written[position] = _screen_written[position];

    // do write.
    digitalWrite(_address_pins[0], ((3-position)>>1) & 0x01);
    digitalWrite(_address_pins[1], (3-position) & 0x01);
    for (uint8_t i=0; i<DATA_PINS_SIZE; i++) {
      digitalWrite(_data_pins[i], (_screen_written[position]>>i) & 0x01);
    }
    delayMicroseconds(1);
    digitalWrite(_write_pin, LOW);
    delayMicroseconds(1);
    digitalWrite(_write_pin, HIGH);
  }
}

void Hpdl1414Writer::set_message(char* msg) {
  uint8_t msg_len = strlen(msg);
  for (uint8_t i=0; i<4; i++) {
    _screen_chars[i] = i<msg_len ? (char) msg[i] : ' ';
  }
}

void Hpdl1414Writer::set_luminosity(uint8_t luminosity) {
  _fade_in = false;
  _fade_out = false;
  for (uint8_t i=0; i<4; i++) {
    _set_luminosity(luminosity, i);
  }
}

void Hpdl1414Writer::_set_luminosity(uint8_t luminosity, uint8_t index) {
  _screen_luminosity[index] = (luminosity>MAX_LUMINOSITY) ? MAX_LUMINOSITY : luminosity;
  _screen_luminosity_pattern[index] = pgm_read_word(LUMINOSITY_PATTERNS + _screen_luminosity[index]);
}

void Hpdl1414Writer::fade_in(uint8_t luminosity) {
  _fade_in_out_start_time_ms = millis();
  _fade_in = true;
  for (uint8_t i=0; i<4; i++) {
    _fade_max_luminosity[i] = luminosity;
  }
  _fade_out = false;
}

void Hpdl1414Writer::fade_out() {
  _fade_in_out_start_time_ms = millis();
  for (uint8_t i=0; i<4; i++) {
    _fade_max_luminosity[i] = _screen_luminosity[i];
  }
  _fade_in = false;
  _fade_out = true;
}

bool Hpdl1414Writer::is_fading() {
  return _fade_in || _fade_out;
}

void Hpdl1414Writer::set_fade_in_out_chars(uint8_t fade_in_out_chars) {
  _fade_in_out_chars = fade_in_out_chars;
}

void Hpdl1414Writer::set_fade_in_out_time_ms(uint16_t fade_in_out_time_ms) {
  _fade_in_out_time_ms = fade_in_out_time_ms;
}

// Function that maps a value x from range [in_start, in_end] to range [out_start, out_end]
int Hpdl1414Writer::_map_value(int x, int in_start, int in_end, int out_start, int out_end) {
    float ratio = (float)(x - in_start) / (in_end - in_start);
    return out_start + (int)((out_end - out_start) * ratio);
}

void Hpdl1414Writer::_update_luminosity_on_fade_in_out() {
  if (!_fade_in && !_fade_out) {
    return;
  }

  uint32_t elapsed_ms = millis() - _fade_in_out_start_time_ms;
  if (elapsed_ms>_fade_in_out_time_ms) {
    for (uint8_t i=0; i<4; i++) {
      if ((_fade_in_out_chars >> (3-i)) & 0x1) {
        uint8_t luminosity = _fade_in ? _fade_max_luminosity[i] : 0;
        _set_luminosity(luminosity, i);
      }
    }
    _fade_in = false;
    _fade_out = false;

  } else {
    for (uint8_t i=0; i<4; i++) {
      if ((_fade_in_out_chars >> (3-i)) & 0x1) {
        uint8_t from = _fade_in ? 1 : _fade_max_luminosity[i];
        uint8_t to = _fade_in ? _fade_max_luminosity[i] : 0;
        uint8_t luminosity = _map_value(elapsed_ms, 0, _fade_in_out_time_ms, from, to);
        _set_luminosity(luminosity, i);
      }
    }
  }
}

void Hpdl1414Writer::blink(uint16_t time_on_ms, uint16_t time_off_ms) {
  _blink_time_on_ms = time_on_ms;
  _blink_time_off_ms = time_off_ms;
  _blink_start_time_ms = millis();
}

bool Hpdl1414Writer::is_blinking() {
  return _blink_start_time_ms > 0;
}

void Hpdl1414Writer::blink_off() {
  _blink_time_on_ms = 0;
  _blink_time_off_ms = 0;
  _blink_start_time_ms = 0;
  _blinking_chars = 0x00;
}

void Hpdl1414Writer::set_blinking_chars(uint8_t blinking_chars) {
  _blinking_chars = blinking_chars;
}