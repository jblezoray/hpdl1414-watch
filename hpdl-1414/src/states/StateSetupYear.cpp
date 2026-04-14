
#include "StateSetupYear.h"

StateSetupYear::StateSetupYear(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupYear::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupYear::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupYear::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupYear::_read(Context* context) {
  return context->rtc->getYear();
}

void StateSetupYear::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "Y %02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupYear::_increment(Context* context, int value) {
  return value >= 99 ? 20 : value +1;
}

void StateSetupYear::_save(Context* context, int value) {
  context->rtc->setYear(value);
}

void StateSetupYear::_on_finish(Context* context) {
  context->state_manager->transition_to(StateCode::STATE_SETUP_MONTH, context);
}