
#include "MixinMoveToSetTemperature.h"

void MixinMoveToSetTemperature::on_enter(Context* context) {
  context->bt1->enable_long_press(3000);
}

void MixinMoveToSetTemperature::in_loop(Context* context) {
  if (context->bt1->is_long_press_event()) {
    context->state_manager->transition_to(StateCode::STATE_SHOW_TEMPERATURE, context);
  }
}

void MixinMoveToSetTemperature::on_exit(Context* context) {
  context->bt1->disable_long_press();
}
