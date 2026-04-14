#ifndef StateSetupHours_h
#define StateSetupHours_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"
#include "mixins/MixinSetValue.h"

class StateSetupHours : public MixinTimeoutToScreenOff, public MixinSetValue<int> {
  public:
    StateSetupHours();
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  protected:
    int _read(Context* context);
    void _print(Context* context, int value);
    int _increment(Context* context, int value);
    void _save(Context* context, int value);
    void _on_finish(Context* context);
};

#endif
