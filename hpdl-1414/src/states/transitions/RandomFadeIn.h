#ifndef RandomFadeIn_h
#define RandomFadeIn_h

#include "AbstractTransition.h"
#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class RandomFadeIn : public AbstractTransition {
  public:
    RandomFadeIn() {};
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
    bool finished(Context* context);
  private:
    uint32_t _start_time_ms;
    uint32_t _last_change_time;
};

#endif