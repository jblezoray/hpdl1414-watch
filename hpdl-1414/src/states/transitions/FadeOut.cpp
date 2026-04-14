
#include "FadeOut.h"

const uint16_t FADE_OUT_TIME = 300;

void FadeOut::on_enter(Context* context) {
  context->hpdl1414->set_fade_in_out_time_ms(FADE_OUT_TIME);
  context->hpdl1414->fade_out();
  _start_time_ms = millis();
  _last_change_time = _start_time_ms;
}

void FadeOut::in_loop(Context* context) {
}

void FadeOut::on_exit(Context* context) {
  context->hpdl1414->set_message("    ");
}

bool FadeOut::finished(Context* context) {
  return millis() - _start_time_ms > FADE_OUT_TIME;
}
