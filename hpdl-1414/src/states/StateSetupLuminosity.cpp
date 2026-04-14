
#include "StateSetupLuminosity.h"

StateSetupLuminosity::StateSetupLuminosity(
) : MixinTimeoutToScreenOff(10000)
{}

void StateSetupLuminosity::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<int>::on_enter(context);
}

void StateSetupLuminosity::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<int>::in_loop(context);
}

void StateSetupLuminosity::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<int>::on_exit(context);
}

int StateSetupLuminosity::_read(Context* context) {
  Configuration c = context->persist->read();
  return c.prefered_luminosity;
}

void StateSetupLuminosity::_print(Context* context, int value) {
  char the_string[5];
  snprintf(the_string, sizeof(the_string), "LM%02d", value);
  context->hpdl1414->set_message(the_string);
}

int StateSetupLuminosity::_increment(Context* context, int value) {
  return value >= MAX_LUMINOSITY ? 1 : value + 1;
}

void StateSetupLuminosity::_save(Context* context, int value) {
  Configuration c = context->persist->read();
  c.prefered_luminosity = value;
  context->persist->save(c);
  context->hpdl1414->set_luminosity(value);
}

void StateSetupLuminosity::_on_finish(Context* context) {
  context->state_manager->transition_to(StateCode::STATE_SETUP_SWITCH_ON_TRANSITION, context);
}