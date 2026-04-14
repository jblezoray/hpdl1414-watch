
#include "TextSlide.h"


const uint16_t TEXT_TIME = 500;

void TextSlide::on_enter(Context* context) {
  context->hpdl1414->set_luminosity(context->persist->read().prefered_luminosity);

  Configuration configuration = context->persist->read();
  strcpy(_buffered_screen, "        ");
  for (int i=0; i<4; i++) {
    _buffered_screen[i+4] = configuration.switch_on_text[i];
  }
  _start_time_ms = millis();
  _next_step_time = _start_time_ms;
}

void TextSlide::in_loop(Context* context) {
  if (millis() >= _next_step_time) {

    // slide the screen left.
    uint8_t len = strlen(_buffered_screen);
    for (int i=0; i<len; i++) {
      _buffered_screen[i] = (i == len-1) ? ' ' : _buffered_screen[i+1];
    }

    // print
    context->hpdl1414->set_message(_buffered_screen);

    _next_step_time += TEXT_TIME / len;
  }
}

void TextSlide::on_exit(Context* context) {
  context->hpdl1414->set_luminosity(0);
  context->hpdl1414->set_fade_in_out_time_ms(100);
  context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
}

bool TextSlide::finished(Context* context) {
  return millis() - _start_time_ms > TEXT_TIME;
}
