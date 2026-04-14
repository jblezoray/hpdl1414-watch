#ifndef TimeFadeIn_h
#define TimeFadeIn_h

#include "AbstractTransition.h"
#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class TimeFadeIn : public AbstractTransition {
  public:
    TimeFadeIn() {};
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
    bool finished(Context* context);
  private:
    uint32_t _start_time_ms;
};

#endif