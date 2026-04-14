#ifndef StateShowSeconds_h
#define StateShowSeconds_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"

class StateShowSeconds : public MixinTimeoutToScreenOff {
  public:
    StateShowSeconds();
    void on_enter(Context * context);
    void in_loop(Context * context);
    void on_exit(Context * context);
  private:
    uint32_t _zero_time;
};

#endif
