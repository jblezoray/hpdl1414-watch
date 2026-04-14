#ifndef StateShowTime_h
#define StateShowTime_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"
#include "mixins/MixinMoveToSetTemperature.h"
#include "mixins/MixinMoveToSetYear.h"

class StateShowTime : public MixinTimeoutToScreenOff,
                      public MixinMoveToSetTemperature,
                      public MixinMoveToSetYear {
  public:
    StateShowTime();
    void on_enter(Context * context);
    void in_loop(Context * context);
    void on_exit(Context * context);
};

#endif
