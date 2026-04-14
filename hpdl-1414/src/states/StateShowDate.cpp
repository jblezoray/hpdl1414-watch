
#include "StateShowDate.h"

StateShowDate::StateShowDate(
) : MixinTimeoutToScreenOff(5000)
{}

void StateShowDate::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinMoveToSetTemperature::on_enter(context);
  MixinMoveToSetYear::on_enter(context);
  if (context->bt2->is_pressed()) {
    context->bt2->ignore_next_released_event();
  }
}

void StateShowDate::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinMoveToSetTemperature::in_loop(context);
  MixinMoveToSetYear::in_loop(context);

  char the_string[5];
  bool century;
  snprintf(
    the_string,
    sizeof(the_string),
    "%2d%2d",
    context->rtc->getMonth(century),
    context->rtc->getDate()
  );
  context->hpdl1414->set_message(the_string);

  if (context->bt2->is_released_event()) {
    context->state_manager->transition_to(StateCode::STATE_SHOW_SECONDS, context);
  }
}

void StateShowDate::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinMoveToSetTemperature::on_exit(context);
  MixinMoveToSetYear::on_exit(context);
}