#ifndef AbstractState_h
#define AbstractState_h

class Context;

class AbstractState {
  public:
    AbstractState() {};
    virtual void on_enter(Context* context) = 0;
    virtual void in_loop(Context* context) = 0;
    virtual void on_exit(Context* context) = 0;
};

#endif