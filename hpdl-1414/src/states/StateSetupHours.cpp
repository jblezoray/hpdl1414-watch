
#include "StateSetupHours.h"

StateSetupHours::StateSetupHours(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupHours::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupHours::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupHours::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupHours::_read(Context* context) {
  bool h12Flag;
  bool pmFlag;
  return context->rtc->getHour(h12Flag, pmFlag);
}

void StateSetupHours::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "HO%02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupHours::_increment(Context* context, int value) {
  return value >= 23 ? 0 : value + 1;
}

void StateSetupHours::_save(Context* context, int value) {
  context->rtc->setHour(value);
}

void StateSetupHours::_on_finish(Context* context) {
  context->state_manager->transition_to(StateCode::STATE_SETUP_MINS, context);
}