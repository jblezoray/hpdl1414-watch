
#include "MixinTimeoutToScreenOff.h"

MixinTimeoutToScreenOff::MixinTimeoutToScreenOff(
  uint32_t show_duration_ms
) : _show_duration_ms(show_duration_ms)
{}

void MixinTimeoutToScreenOff::on_enter(Context* context) {
  _show_start_time_ms = millis();
}

void MixinTimeoutToScreenOff::in_loop(Context* context) {
  uint32_t now = millis();

  if (
    context->bt1->is_pressed()
    || context->bt2->is_pressed()
  ) {
    _show_start_time_ms = now;
  }

  if (now - _show_start_time_ms >= _show_duration_ms) {
    context->state_manager->transition_to(
      StateCode::STATE_SCREEN_OFF, context, TransitionCode::TRANSITION_FADE_OUT
    );
  }
}

void MixinTimeoutToScreenOff::on_exit(Context* context) {
}
