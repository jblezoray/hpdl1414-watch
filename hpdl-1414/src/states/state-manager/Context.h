#ifndef Context_h
#define Context_h

#include "StateManager.h"

#include "../../button/Button.h"
#include "../../screen-hpdl1414/Hpdl1414Writer.h"
#include "../../rtc-ds3231/DS3231.h"
#include "../../vcc-controller/VccController.h"
#include "../../persist-configuration/Persist.h"
#include "../../persist-configuration/Configuration.h"

class StateManager;

class Context {
   public:
    Context(
      StateManager* state_manager,
      Hpdl1414Writer* hpdl1414,
      Button* bt1,
      Button* bt2,
      DS3231* rtc,
      VccController* vccController,
      Persist<Configuration>* persist,
      uint32_t updateStateIntervalMs,
      uint32_t updateScreenIntervalMs
    );
    StateManager* state_manager;
    Hpdl1414Writer* hpdl1414;
    Button* bt1;
    Button* bt2;
    DS3231* rtc;
    VccController* vccController;
    Persist<Configuration>* persist;
    uint32_t updateStateIntervalMs;
    bool updateStateBurst;
    uint32_t lastStateUpdateTime;
    uint32_t updateScreenIntervalMs;
    bool updateScreenBurst;
    uint32_t lastScreenUpdateTime;
};

#endif