
#include "StateSetupDay.h"

StateSetupDay::StateSetupDay(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupDay::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupDay::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupDay::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupDay::_read(Context* context) {
  return context->rtc->getDate();
}

void StateSetupDay::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "D %02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupDay::_increment(Context* context, int value) {
  bool century;
  int month = context->rtc->getMonth(century);
  int max_value = 0;
  switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      max_value = 31;
      break;
    case 4: case 6: case 9: case 11:
      max_value = 30;
      break;
    case 2:
      int year = 2000 + context->rtc->getYear();
      if (year % 400 == 0) {
        max_value = 29;
      } else if (year % 100 == 0) {
        max_value = 28;
      } else if (year % 4 == 0) {
        max_value = 29;
      } else {
        max_value = 28;
      }
      break;
  }
  return value >= max_value ? 1 : value + 1;
}

void StateSetupDay::_save(Context* context, int value) {
    context->rtc->setDate(value);
}

void StateSetupDay::_on_finish(Context* context) {
    context->state_manager->transition_to(StateCode::STATE_SETUP_HOURS, context);
}