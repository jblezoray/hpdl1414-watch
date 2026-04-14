#include "StateSetupSwitchOnText.h"

StateSetupSwitchOnText::StateSetupSwitchOnText() : MixinTimeoutToScreenOff(10000)
{
  _currently_edited_char_index = 0;
}

void StateSetupSwitchOnText::_read(Context* context, char (&out)[5]) {
  Configuration configuration = context->persist->read();
  for (int i=0; i<4; i++) {
    out[i] = configuration.switch_on_text[i];
  }
  out[4] = 0;
}

void StateSetupSwitchOnText::_save(Context* context) {
  Configuration configuration = context->persist->read();
  for (int i=0; i<4; i++) {
    configuration.switch_on_text[i] = _text[i];
  }
  context->persist->save(configuration);
}

void StateSetupSwitchOnText::_update_blinking_char(Context* context, int nth_char) {
  uint8_t blinking_char = 1 << (3 - nth_char); // 0b1000, 0b0100...
  context->hpdl1414->set_blinking_chars(blinking_char);
}

void StateSetupSwitchOnText::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  _read(context, _text);
  context->bt1->enable_repeated_press(750, 150);
  _update_blinking_char(context, _currently_edited_char_index);
}

void StateSetupSwitchOnText::in_loop(Context* context) {
  if (context->bt1->is_pressed() && context->hpdl1414->is_blinking()) {
    context->hpdl1414->blink_off();
  } else if (!context->bt1->is_pressed() && !context->hpdl1414->is_blinking()) {
    context->hpdl1414->blink(300, 300);
  }

  // force a refresh from times to times
  if (millis() % 1000 == 0) {
    _read(context, _text);
  }

  context->hpdl1414->set_message(_text);

  if (context->bt1->is_pressed_event()) {
    _text[_currently_edited_char_index] += 1;
    if (_text[_currently_edited_char_index] > 0x5F) {
      _text[_currently_edited_char_index] = 0x20;
    }
    _save(context);
  }

  if (!context->bt2->is_long_press_event() && context->bt2->is_pressed_event()) {
    if (_currently_edited_char_index < 3) {
      _currently_edited_char_index += 1;
      _update_blinking_char(context, _currently_edited_char_index);
    } else {
      Configuration configuration = context->persist->read();
      context->state_manager->transition_to(
        StateCode::STATE_SHOW_TIME,
        context,
        configuration.switch_on_transition
      );
    }
  }
}

void StateSetupSwitchOnText::on_exit(Context* context) {
  context->bt1->disable_repeated_press();
  context->hpdl1414->blink_off();
}
