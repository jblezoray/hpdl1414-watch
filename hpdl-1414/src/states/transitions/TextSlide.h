#ifndef TextSlide_h
#define TextSlide_h

#include "AbstractTransition.h"
#include "../state-manager/AbstractState.h"
#include "../state-manager/Context.h"

class TextSlide : public AbstractTransition {
  public:
    TextSlide() {};
    void on_enter(Context* context);
    void in_loop(Context* context);
    void on_exit(Context* context);
    bool finished(Context* context);
  private:
    static const uint8_t BUFFER_SIZE = 9; // 8 chars + null terminator
    uint32_t _start_time_ms;
    uint32_t _next_step_time;
    char _buffered_screen[BUFFER_SIZE];
};

#endif