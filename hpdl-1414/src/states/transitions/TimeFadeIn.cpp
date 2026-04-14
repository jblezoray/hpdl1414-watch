
#include "TimeFadeIn.h"


const uint16_t TIME_FADE_IN_TIME = 700;

void TimeFadeIn::on_enter(Context* context) {
  char the_string[5];
  bool h12Flag;
  bool pmFlag;
  snprintf(
    the_string,
    sizeof(the_string),
    "%2d%02d",
    context->rtc->getHour(h12Flag, pmFlag),
    context->rtc->getMinute()
  );
  context->hpdl1414->set_message(the_string);

  context->hpdl1414->set_luminosity(0);
  context->hpdl1414->set_fade_in_out_time_ms(TIME_FADE_IN_TIME);
  context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
  _start_time_ms = millis();
}

void TimeFadeIn::in_loop(Context* context) {
}

void TimeFadeIn::on_exit(Context* context) {
}

bool TimeFadeIn::finished(Context* context) {
  return millis() - _start_time_ms > TIME_FADE_IN_TIME;
}
