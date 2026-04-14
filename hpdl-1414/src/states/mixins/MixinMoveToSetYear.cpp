
#include "MixinMoveToSetYear.h"

void MixinMoveToSetYear::on_enter(Context* context) {
  context->bt2->enable_long_press(1000);
}

void MixinMoveToSetYear::in_loop(Context* context) {
  if (context->bt2->is_long_press_event()) {
    context->state_manager->transition_to(StateCode::STATE_SETUP_YEAR, context);
  }
}

void MixinMoveToSetYear::on_exit(Context* context) {
  context->bt2->disable_long_press();
}
