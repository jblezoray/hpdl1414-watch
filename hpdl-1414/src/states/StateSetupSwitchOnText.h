#ifndef StateSetupSwitchOnText_h
#define StateSetupSwitchOnText_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"
#include "mixins/MixinSetValue.h"

class StateSetupSwitchOnText : public MixinTimeoutToScreenOff {
  public:
    StateSetupSwitchOnText();
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  private:
    int _currently_edited_char_index;
    char _text[5] = {'*','*','*','*','\0'};
    void _read(Context* context, char (&out)[5]);
    void _save(Context* context);
    void _update_blinking_char(Context* context, int nth_char);
};

#endif
