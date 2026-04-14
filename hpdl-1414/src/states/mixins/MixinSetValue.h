#ifndef MixinSetValue_h
#define MixinSetValue_h

#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

template <typename T>
class MixinSetValue : virtual public AbstractState {
  public:
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  protected:
    T _value;
    virtual T _read(Context* context) = 0;
    virtual void _print(Context* context, T value) = 0;
    virtual T _increment(Context* context, T value) = 0;
    virtual void _save(Context* context, T value) = 0;
    virtual void _on_finish(Context* context) = 0;
};

template <typename T>
void MixinSetValue<T>::on_enter(Context* context) {
  context->hpdl1414->set_message("    ");
  _value = _read(context);
  context->bt1->enable_repeated_press(750, 150);
  context->hpdl1414->set_blinking_chars(0b0011);
}

template <typename T>
void MixinSetValue<T>::in_loop(Context* context) {
  if (context->bt1->is_pressed() && context->hpdl1414->is_blinking()) {
    context->hpdl1414->blink_off();
  } else if (!context->bt1->is_pressed() && !context->hpdl1414->is_blinking()) {
    context->hpdl1414->blink(300, 300);
  }

  // force a refresh from times to times
  if (millis() % 1000 == 0) {
    _value = _read(context);
  }

  _print(context, _value);

  if (context->bt1->is_pressed_event()) {
    _value = _increment(context, _value);
    _save(context, _value);
  }
  if (!context->bt2->is_long_press_event() && context->bt2->is_pressed_event()) {
    _on_finish(context);
  }
}

template <typename T>
void MixinSetValue<T>::on_exit(Context* context) {
  context->bt1->disable_repeated_press();
  context->hpdl1414->blink_off();
}

#endif
