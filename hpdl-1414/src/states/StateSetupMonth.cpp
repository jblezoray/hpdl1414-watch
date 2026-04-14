
#include "StateSetupMonth.h"

StateSetupMonth::StateSetupMonth(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupMonth::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupMonth::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupMonth::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupMonth::_read(Context* context) {
  bool century;
  return context->rtc->getMonth(century);
}

void StateSetupMonth::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "M %02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupMonth::_increment(Context* context, int value) {
  return value >= 12 ? 1 : value + 1;
}

void StateSetupMonth::_save(Context* context, int value) {
    context->rtc->setMonth(value);
}

void StateSetupMonth::_on_finish(Context* context) {
    context->state_manager->transition_to(StateCode::STATE_SETUP_DAY, context);
}