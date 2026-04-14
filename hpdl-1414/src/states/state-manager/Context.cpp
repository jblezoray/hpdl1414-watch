

#include "Context.h"



Context::Context(
  StateManager* state_manager,
  Hpdl1414Writer* hpdl1414,
  Button* bt1,
  Button* bt2,
  DS3231* rtc,
  VccController* vccController,
  Persist<Configuration>* persist,
  uint32_t updateStateIntervalMs,
  uint32_t updateScreenIntervalMs
) : state_manager(state_manager), 
    hpdl1414(hpdl1414), 
    bt1(bt1), 
    bt2(bt2), 
    rtc(rtc),
    vccController(vccController),
    persist(persist),
    updateStateIntervalMs(updateStateIntervalMs),
    updateScreenIntervalMs(updateScreenIntervalMs)
{
  updateStateBurst = false;
  updateScreenBurst = false;
  lastStateUpdateTime = 0;
  lastScreenUpdateTime = 0;
}
