
#include "StateShowTemperature.h"

StateShowTemperature::StateShowTemperature(
) : MixinTimeoutToScreenOff(5000)
{}

void StateShowTemperature::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  if (context->bt2->is_pressed()) {
    context->bt2->ignore_next_released_event();
  }
  if (context->bt1->is_pressed()) {
    context->bt1->ignore_next_released_event();
  }
}

void StateShowTemperature::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);

  float temperature_celsius = context->rtc->getTemperature();

  char the_string[5];
  if (context->persist->read().temperature_in_celsius) {
    snprintf(
      the_string,
      sizeof(the_string),
      "%+03dC",
      (int)temperature_celsius
    );
  } else {
    snprintf(
      the_string,
      sizeof(the_string),
      "%+03dF",
      (int)(temperature_celsius * 1.8 + 32.0)
    );
  }

  context->hpdl1414->set_message(the_string);

  if (context->bt1->is_released_event()) {
    Configuration conf = context->persist->read();
    conf.temperature_in_celsius = ! conf.temperature_in_celsius;
    context->persist->save(conf);
  }
  if (context->bt2->is_released_event()) {
    context->state_manager->transition_to(StateCode::STATE_SHOW_TIME, context);
  }
}

void StateShowTemperature::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
}