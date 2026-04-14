
#include "RandomFadeIn.h"


const uint16_t RANDOM_FADE_IN_TIME = 400;
const uint16_t RANDOM_CHANGE_TIME = 25;

void RandomFadeIn::on_enter(Context* context) {
  context->updateScreenBurst = true;
  context->updateStateBurst = true;
  context->hpdl1414->set_fade_in_out_time_ms(RANDOM_FADE_IN_TIME);
  context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
  _start_time_ms = millis();
  _last_change_time = _start_time_ms;
}

void RandomFadeIn::in_loop(Context* context) {
  uint32_t timestamp = millis();
  if (timestamp - _last_change_time > RANDOM_CHANGE_TIME) {
    char message[4];
    for (int i=0; i<4; i++) {
      message[i] = (char)(random(32, 96)); // all chars
      // message[i] = (char)(random(48, 57)); // numbers only
    }
    context->hpdl1414->set_message(message);
    _last_change_time = timestamp;
  }
}

void RandomFadeIn::on_exit(Context* context) {
  context->updateScreenBurst = false;
  context->updateStateBurst = false;
}

bool RandomFadeIn::finished(Context* context) {
  return millis() - _start_time_ms > RANDOM_FADE_IN_TIME;
}
