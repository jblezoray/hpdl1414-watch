#include "Arduino.h"
#include "Button.h"

Button::Button(uint8_t button_pin) {
  _button_pin = button_pin;
  _previous_state = -1;
  _state = -1;
  _physical_press_event_start_time_ms = 0;

  _repeated_press_initial_delay_ms = 0;
  _repeated_press_events_period_ms = 0;
  _repeated_press_next_event_time_ms = 0;
  _is_repeated_press_event = false;

  _ignore_next_released_event = false;

  _long_press_delay_ms = 0;
  _is_long_press_event = false;
  _long_press_event_fired = false;
}

void Button::setup() {
  pinMode(_button_pin, INPUT_PULLUP);
}

void Button::update_state() {
  int8_t read_state = digitalRead(_button_pin);
  _previous_state = _previous_state==-1 ? read_state : _state;
  _state = read_state;

  // record physical press events start times
  if (is_released() && _physical_press_event_start_time_ms > 0) {
    _physical_press_event_start_time_ms = 0;
  } else if (_is_physicaly_pressed_event()) {
    _physical_press_event_start_time_ms = millis();
  }

  // repeated press management.
  _is_repeated_press_event = false;
  if (_repeated_press_initial_delay_ms>0 && _repeated_press_events_period_ms>0){
    // set start time
    if (_is_physicaly_pressed_event()) {
      _repeated_press_next_event_time_ms = _physical_press_event_start_time_ms + _repeated_press_initial_delay_ms;
    }
    // record awaiting event.
    if (_physical_press_event_start_time_ms > 0) {
      uint32_t cur_time_ms = millis();
      if (cur_time_ms>_repeated_press_next_event_time_ms) {
        _repeated_press_next_event_time_ms = cur_time_ms + _repeated_press_events_period_ms;
        _is_repeated_press_event = true;
      }
    }
  }

  // long press management
  _is_long_press_event = false;
  if (_long_press_delay_ms>0 && !_long_press_event_fired && is_pressed()) {
    uint32_t cur_time_ms = millis();
    if (cur_time_ms> _physical_press_event_start_time_ms + _long_press_delay_ms) {
      _is_long_press_event = true;
      _long_press_event_fired = true;
    }
  }
  if (_long_press_event_fired && is_released()) {
    _long_press_event_fired = false;
  }
}

bool Button::is_pressed(){
  return _state == LOW;
}

bool Button::is_released(){
  return _state == HIGH;
}

bool Button::is_long_pressed() {
  return _long_press_event_fired;
}

bool Button::block_until_pressed() {
  while (!is_pressed_event()) {
    update_state();
  }
}

bool Button::is_pressed_event(){
  return _is_physicaly_pressed_event() || _is_repeated_press_event;
}

bool Button::is_released_event(){
  if (!_is_physicaly_released_event()) {
    return false;
  }
  if (_ignore_next_released_event) {
    _ignore_next_released_event = false;
    return false;
  }
  return true;
}

bool Button::is_long_press_event() {
  return _is_long_press_event;
}

void Button::ignore_next_released_event() {
  _ignore_next_released_event = true;
}

bool Button::_is_physicaly_pressed_event(){
  return _state == LOW && _previous_state == HIGH;
}

bool Button::_is_physicaly_released_event(){
  return _state == HIGH && _previous_state == LOW;
}

void Button::enable_repeated_press(uint16_t initial_delay_ms, uint16_t events_period_ms) {
  _repeated_press_initial_delay_ms = initial_delay_ms;
  _repeated_press_events_period_ms = events_period_ms;
}

void Button::disable_repeated_press() {
  _repeated_press_initial_delay_ms = 0;
  _repeated_press_events_period_ms = 0;
}

void Button::enable_long_press(uint16_t delay_ms) {
  _long_press_delay_ms = delay_ms;
}

void Button::disable_long_press() {
  _long_press_delay_ms = 0;
}