#ifndef MixinMoveToSetYear_h
#define MixinMoveToSetYear_h

#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class MixinMoveToSetYear : virtual public AbstractState {
  public:
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
};

#endif
