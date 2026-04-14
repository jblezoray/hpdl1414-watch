
#include "TextFadeIn.h"


const uint16_t TEXT_FADE_IN_TIME = 700;

void TextFadeIn::on_enter(Context* context) {
  Configuration configuration = context->persist->read();
  char message[5];
  for (int i=0; i<5; i++) {
    message[i] = configuration.switch_on_text[i];
  }
  message[4] = 0;

  context->hpdl1414->set_message(message);
  context->hpdl1414->set_luminosity(0);
  context->hpdl1414->set_fade_in_out_time_ms(TEXT_FADE_IN_TIME);
  context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
  _start_time_ms = millis();
}

void TextFadeIn::in_loop(Context* context) {
}

void TextFadeIn::on_exit(Context* context) {
}

bool TextFadeIn::finished(Context* context) {
  return millis() - _start_time_ms > TEXT_FADE_IN_TIME;
}
