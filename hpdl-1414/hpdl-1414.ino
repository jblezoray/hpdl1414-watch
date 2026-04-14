#include "Arduino.h"
#include "src/screen-hpdl1414/Hpdl1414Writer.h"
#include "src/button/Button.h"
#include "src/blinker/Blinker.h"
#include "src/persist-configuration/Persist.h"
#include "src/persist-configuration/Configuration.h"
#include "src/rtc-ds3231/DS3231.h"
#include "src/vcc-controller/VccController.h"
#include "src/states/state-manager/StateManager.h"
#include "src/states/state-manager/Context.h"
#include "src/pins.h"

const uint8_t ADDRESS_PINS[] = {PIN_A1, PIN_A0};
const uint8_t DATA_PINS[] = {PIN_D0, PIN_D1, PIN_D2, PIN_D3, PIN_D4, PIN_D5, PIN_D6};

Persist<Configuration> persist(PIN_EEPROM);
Hpdl1414Writer hpdl1414(DATA_PINS, ADDRESS_PINS, PIN_WR);
Button bt1(PIN_BT1);
Button bt2(PIN_BT2);
DS3231 rtc;
VccController vccController(PIN_VCC_CONTROLLER);
Blinker blinker(PIN_LED);

StateManager stateManager;
Context context(
  &stateManager,
  &hpdl1414,
  &bt1,
  &bt2,
  &rtc,
  &vccController,
  &persist,
  20, // state update = 50Hz
  2   // screen update = 500Hz
);

bool setup_done = false;

void setup() {
  Wire.begin(); // for DS3231.
  persist.setup();
  if (!persist.is_valid()) {
    Configuration c{MAX_LUMINOSITY, true, true, TransitionCode::TRANSITION_RANDOM_FADE_IN, {'*', '*', '*', '*'}};
    persist.save(c);
  }
  hpdl1414.setup(persist.read().prefered_luminosity);
  bt1.setup();
  bt2.setup();
  stateManager.transition_to(StateCode::STATE_SCREEN_OFF, &context);
  setup_done = true;
}

void loop() {
  if (!setup_done) return;

  uint32_t currentTime = millis();

  if (
    currentTime - context.lastStateUpdateTime >= context.updateStateIntervalMs
    || context.updateStateBurst
  ) {
    context.lastStateUpdateTime = currentTime;
    bt1.update_state();
    bt2.update_state();
    stateManager.in_loop(&context);
  }

  if (
    currentTime - context.lastScreenUpdateTime >= context.updateScreenIntervalMs
    || context.updateScreenBurst
  ) {
    context.lastScreenUpdateTime = currentTime;
    hpdl1414.refresh_screen();
  }

  blinker.update();
}