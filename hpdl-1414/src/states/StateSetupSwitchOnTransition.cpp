#include "StateSetupSwitchOnTransition.h"

StateSetupSwitchOnTransition::StateSetupSwitchOnTransition(
) : MixinTimeoutToScreenOff(10000)
{
  _transition_code_demo = TransitionCode::TRANSITION_NONE;
}

void StateSetupSwitchOnTransition::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  MixinSetValue<TransitionCode>::on_enter(context);
  context->hpdl1414->blink_off();
}

void StateSetupSwitchOnTransition::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);
  MixinSetValue<TransitionCode>::in_loop(context);
}

void StateSetupSwitchOnTransition::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
  MixinSetValue<TransitionCode>::on_exit(context);
}

TransitionCode StateSetupSwitchOnTransition::_read(Context* context) {
  Configuration configuration = context->persist->read();
  return configuration.switch_on_transition;
}

void StateSetupSwitchOnTransition::_step_in_transition_code(Context* context, TransitionCode value) {
  switch (value) {
    // case TransitionCode::TRANSITION_NONE: break;
    case TransitionCode::TRANSITION_TIME_FADE_IN:
      context->hpdl1414->set_message("ON24");
      context->hpdl1414->set_fade_in_out_chars(0b0011);
      break;
    case TransitionCode::TRANSITION_RANDOM_FADE_IN:
      break;
    case TransitionCode::TRANSITION_TEXT_FADE_IN:
      context->hpdl1414->set_message("ONAB");
      context->hpdl1414->set_fade_in_out_chars(0b0011);
      break;
    default: break;
  }
}

void StateSetupSwitchOnTransition::_step_out_transition_code(Context* context, TransitionCode value) {
  switch (_transition_code_demo) {
    // case TransitionCode::TRANSITION_NONE: break;
    case TransitionCode::TRANSITION_TIME_FADE_IN:
    case TransitionCode::TRANSITION_TEXT_FADE_IN:
      context->hpdl1414->set_fade_in_out_chars(0b1111);
      context->hpdl1414->set_luminosity(context->persist->read().prefered_luminosity);
      break;
    case TransitionCode::TRANSITION_RANDOM_FADE_IN:
      break;
      context->hpdl1414->set_fade_in_out_chars(0b1111);
      break;
    default: break;
  }
}

void StateSetupSwitchOnTransition::_print(Context* context, TransitionCode value) {
  if (_transition_code_demo != value) {
    _step_out_transition_code(context, value);
    _step_in_transition_code(context, value);
    _transition_code_demo = value;
  }

  // increment the current transition code demo.
  char message[5] = "ON..";
  switch (value) {
    case TransitionCode::TRANSITION_TIME_FADE_IN:
    case TransitionCode::TRANSITION_TEXT_FADE_IN:
      if (!context->hpdl1414->is_fading()) {
        context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
      }
      break;
    case TransitionCode::TRANSITION_RANDOM_FADE_IN:
      message[2] = (char)(random(32, 96));
      message[3] = (char)(random(32, 96));
      context->hpdl1414->set_message(message);
      break;
    case TransitionCode::TRANSITION_TEXT_SLIDE:
      switch ((millis() / 200) % 4) {
        case 0: // 'ONAB'
          message[2] = 'A';
          message[3] = 'B';
          break;
        case 1: // 'ONB '
          message[2] = 'B';
          message[3] = ' ';
          break;
        case 2: // 'ON  '
          message[2] = ' ';
          message[3] = ' ';
          break;
        case 3: // 'ON A'
          message[2] = ' ';
          message[3] = 'A';
          break;
      }
      context->hpdl1414->set_message(message);
      break;
    default: break;
  }
}

TransitionCode StateSetupSwitchOnTransition::_increment(Context* context, TransitionCode value) {
  switch (value) {
    case TransitionCode::TRANSITION_TIME_FADE_IN:  return TransitionCode::TRANSITION_RANDOM_FADE_IN;
    case TransitionCode::TRANSITION_RANDOM_FADE_IN:return TransitionCode::TRANSITION_TEXT_FADE_IN;
    case TransitionCode::TRANSITION_TEXT_FADE_IN:  return TransitionCode::TRANSITION_TEXT_SLIDE;
    case TransitionCode::TRANSITION_TEXT_SLIDE:    return TransitionCode::TRANSITION_TIME_FADE_IN;
    default: break;
  }
  return TransitionCode::TRANSITION_TIME_FADE_IN;
}

void StateSetupSwitchOnTransition::_save(Context* context, TransitionCode value) {
  Configuration c = context->persist->read();
  c.switch_on_transition = value;
  context->persist->save(c);
}

void StateSetupSwitchOnTransition::_on_finish(Context* context) {
  _step_out_transition_code(context, _transition_code_demo);
  Configuration c = context->persist->read();
  bool has_text = c.switch_on_transition == TransitionCode::TRANSITION_TEXT_FADE_IN
    || c.switch_on_transition == TransitionCode::TRANSITION_TEXT_SLIDE;
  if (has_text) {
    context->state_manager->transition_to(
      StateCode::STATE_SETUP_SWITCH_ON_TEXT,
      context
    );
  } else {
    context->state_manager->transition_to(
      StateCode::STATE_SHOW_TIME,
      context,
      c.switch_on_transition
    );
  }
}