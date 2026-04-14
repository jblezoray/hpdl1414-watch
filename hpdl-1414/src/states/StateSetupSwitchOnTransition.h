#ifndef StateSetupSwitchOnTransition_h
#define StateSetupSwitchOnTransition_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "state-manager/TransitionCode.h"
#include "mixins/MixinTimeoutToScreenOff.h"
#include "mixins/MixinSetValue.h"

class StateSetupSwitchOnTransition : public MixinTimeoutToScreenOff, public MixinSetValue<TransitionCode> {
  public:
    StateSetupSwitchOnTransition();
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  protected:
    TransitionCode _read(Context* context);
    void _print(Context* context, TransitionCode value);
    TransitionCode _increment(Context* context, TransitionCode value);
    void _save(Context* context, TransitionCode value);
    void _on_finish(Context* context);
    TransitionCode _transition_code_demo;
    void _step_in_transition_code(Context* context, TransitionCode value);
    void _step_out_transition_code(Context* context, TransitionCode value);
};

#endif
