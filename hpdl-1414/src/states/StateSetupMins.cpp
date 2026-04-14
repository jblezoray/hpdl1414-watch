
#include "StateSetupMins.h"

StateSetupMins::StateSetupMins(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupMins::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupMins::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupMins::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupMins::_read(Context* context) {
  return context->rtc->getMinute();
}

void StateSetupMins::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "MI%02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupMins::_increment(Context* context, int value) {
  return value >= 59 ? 0 : value + 1;
}

void StateSetupMins::_save(Context* context, int value) {
  context->rtc->setMinute(value);
}

void StateSetupMins::_on_finish(Context* context) {
  context->state_manager->transition_to(StateCode::STATE_SETUP_LUMINOSITY, context);
}