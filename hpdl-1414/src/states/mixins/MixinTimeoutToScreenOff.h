#ifndef MixinTimeoutToScreenOff_h
#define MixinTimeoutToScreenOff_h

#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class MixinTimeoutToScreenOff : virtual public AbstractState {
  public:
    MixinTimeoutToScreenOff(uint32_t show_duration_ms);
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  private:
    uint32_t _show_duration_ms;
    uint32_t _show_start_time_ms;
};

#endif
