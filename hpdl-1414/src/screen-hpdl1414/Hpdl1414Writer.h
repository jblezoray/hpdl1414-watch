#ifndef Hpdl1414Writer_h
#define Hpdl1414Writer_h

#include "Arduino.h"

const PROGMEM uint16_t LUMINOSITY_PATTERNS[] = {
  0b0000000000000000, // 0  0/16
  0b0000100001000001, // 1  3/16
  0b0001000100010001, // 2  4/16
  0b0010010100100101, // 3  6/16
  0b0101010101010101, // 4  8/16
  0b0110110110110110, // 5 10/16
  0b0111101111011111, // 6 13/16
  0b1111111111111111, // 7 16/16
};
const uint8_t MAX_LUMINOSITY = 7;

const uint8_t DATA_PINS_SIZE=7;
const uint8_t ADDRESS_PINS_SIZE=2;


// a driven that can handle an HPDL1414 display.
class Hpdl1414Writer {
  public:

    // data_pins: an array of seven pins identifiers, connected to the "DATA INPUT" pins of the HPDL1414
    // address_pins: an array of two pins identifiers, connected to the "DIGIT SELECT" pins of the HPDL1414
    // write_pin: the pins identifie connected to the "WR" pin of the HPDL1414
    Hpdl1414Writer(
      const uint8_t data_pins[],
      const uint8_t address_pins[],
      const uint8_t write_pin
    );

    void switch_on();
    void switch_off();

    // call this in the "setup" section.
    void setup(int default_luminosity);

    // call this on every loop of the "loop" section
    void refresh_screen();

    // prints a string on the screen.  
    // The string will be padded with spaces.
    void set_message(char* msg);

    // set the luminosity of the screen.
    // the luminosity is an int between 0 and MAX_LUMINOSITY.
    void set_luminosity(uint8_t luminosity);

    // start a fade in / out (change luminosity to make characters appear / disapear)
    void set_fade_in_out_time_ms(uint16_t fade_in_out_time_ms);
    void fade_in(uint8_t luminosity);
    void fade_out();
    bool is_fading();
    void set_fade_in_out_chars(uint8_t fade_in_out_chars);

    // blink the display
    void blink(uint16_t time_on_ms, uint16_t time_off_ms);
    bool is_blinking();
    void blink_off();
    void set_blinking_chars(uint8_t blinking_chars);

  private:
    uint16_t _refresh_screen_counter;

    uint8_t _data_pins[DATA_PINS_SIZE];
    uint8_t _address_pins[ADDRESS_PINS_SIZE];
    uint8_t _write_pin;

    // time taken for a fade in or for a fade out.
    uint16_t _fade_in_out_time_ms;
    // the time a fade in / out started.
    uint32_t _fade_in_out_start_time_ms = 0;
    // are we in a fade in? in a fade out?
    bool _fade_in, _fade_out;
    // the chars that fade in / out
    uint8_t _fade_in_out_chars;
    uint8_t _fade_max_luminosity[4];

    // blink time on and off.
    uint16_t _blink_time_on_ms;
    uint16_t _blink_time_off_ms;
    // the start time of the blinking.
    uint32_t _blink_start_time_ms;
    // blinking chars
    uint8_t _blinking_chars;

    // the chars to print. 
    char _screen_chars[4];
    // what is physically written on the screen.
    // this can be updated before calling _write_screen
    char _screen_written[4];
    // what was previously written on the screen.
    char _previous_screen_written[4];

    uint8_t _screen_luminosity[4];
    uint16_t _screen_luminosity_pattern[4];

    void _update_luminosity_on_fade_in_out();
    int _map_value(int x, int in_start, int in_end, int out_start, int out_end);

    // physically write '_screen_written' on the screen.
    void _write_screen(bool force);

    void _set_luminosity(uint8_t luminosity, uint8_t index);
};

#endif

