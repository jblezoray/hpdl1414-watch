#ifndef AbstractTransition_h
#define AbstractTransition_h

#include "../state-manager/AbstractState.h"

class Context;

class AbstractTransition : virtual public AbstractState {
  public:
    AbstractTransition() {};
    virtual bool finished(Context* context) = 0;
};

#endif