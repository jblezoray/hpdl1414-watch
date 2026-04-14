#ifndef StateShowVoltage_h
#define StateShowVoltage_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"

class StateShowVoltage : public MixinTimeoutToScreenOff {
  public:
    StateShowVoltage();
    void on_enter(Context * context);
    void in_loop(Context * context);
    void on_exit(Context * context);
  private:
    uint16_t _vcc_millivolts;
    uint32_t _last_read_time_ms;
};

#endif
