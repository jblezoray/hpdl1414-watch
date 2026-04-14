#ifndef StateShowTemperature_h
#define StateShowTemperature_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"

class StateShowTemperature : public MixinTimeoutToScreenOff {
  public:
    StateShowTemperature();
    void on_enter(Context * context);
    void in_loop(Context * context);
    void on_exit(Context * context);
};

#endif
