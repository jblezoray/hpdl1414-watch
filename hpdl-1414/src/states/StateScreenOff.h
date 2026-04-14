#ifndef StateScreenOff_h
#define StateScreenOff_h

#include "state-manager/AbstractState.h"
#include "state-manager/Context.h"
#include "mixins/MixinTimeoutToScreenOff.h"

class StateScreenOff : public AbstractState {
  public:
    StateScreenOff();
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
  private:
    void disable_ADC();
    void enable_ADC();
    uint8_t _adc_saved_state;
};

#endif
