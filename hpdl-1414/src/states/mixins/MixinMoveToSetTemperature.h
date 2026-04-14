#ifndef MixinMoveToSetTemperature_h
#define MixinMoveToSetTemperature_h

#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class MixinMoveToSetTemperature : virtual public AbstractState {
  public:
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
};

#endif
