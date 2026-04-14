
#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button {
  public:
    Button(uint8_t button_pin);

    // call this in the "setup" section.
    void setup();

    // call this on every loop of the "loop" section
    void update_state();

    bool is_pressed();
    bool is_released();
    bool is_long_pressed();
    bool is_pressed_event();
    bool is_released_event();
    bool is_long_press_event();

    void ignore_next_released_event();

    // enable repeated press.
    // initial_delay_ms is the delay before a press is considered as long.
    // events_period_ms is the time between two 'is_pressed_event' are fired.
    void enable_repeated_press(uint16_t initial_delay_ms, uint16_t events_period_ms);
    void disable_repeated_press();

    // set long press delay.
    void enable_long_press(uint16_t initial_delay_ms);
    void disable_long_press();

    // Blocks the program flow untill the button is pressed.
    // note that it is not a good idea to call this...
    bool block_until_pressed();

  private:
    uint8_t _button_pin;
    int8_t _previous_state;
    int8_t _state;

    uint32_t _physical_press_event_start_time_ms;

    uint16_t _repeated_press_initial_delay_ms;
    uint16_t _repeated_press_events_period_ms;
    uint32_t _repeated_press_next_event_time_ms;
    bool _is_repeated_press_event;

    bool _ignore_next_released_event;

    uint16_t _long_press_delay_ms;
    bool _is_long_press_event;
    bool _long_press_event_fired;

    bool _is_physicaly_pressed_event();
    bool _is_physicaly_released_event();

};

#endif