
#include "StateShowSeconds.h"

StateShowSeconds::StateShowSeconds(
) : MixinTimeoutToScreenOff(60000)
{}

void StateShowSeconds::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  _zero_time = millis();
}

void StateShowSeconds::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);

  char the_string[5];
  snprintf(
    the_string,
    sizeof(the_string),
    "%2d.%1d",
    context->rtc->getSecond(),
    ((millis() - _zero_time) / 100) % 10
  );
  context->hpdl1414->set_message(the_string);

  if (context->bt1->is_pressed_event()) {
    context->rtc->setSecond(0);
    _zero_time = millis();
  }
  if (context->bt2->is_pressed_event()) {
    context->state_manager->transition_to(StateCode::STATE_SHOW_VOLTAGE, context);
  }
}

void StateShowSeconds::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
}