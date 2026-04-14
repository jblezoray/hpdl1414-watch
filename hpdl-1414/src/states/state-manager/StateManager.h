#ifndef StateManager_h
#define StateManager_h

#include "Arduino.h"

#include "StateCode.h"
#include "TransitionCode.h"
#include "AbstractState.h"
#include "../transitions/AbstractTransition.h"
#include "Context.h"

class AbstractState;

class StateManager {
  public:
    using Creator = AbstractState*(*)();

    StateManager();
    void in_loop(Context* context);
    void transition_to(StateCode new_state_code, Context* context);
    void transition_to(StateCode new_state_code, Context* context, TransitionCode transition_code);

  private:
    AbstractState* _current_state;
    AbstractTransition* _transition_state;
    AbstractState* _next_state;
};

#endif
