#include "StateScreenOff.h"
#include <avr/sleep.h>
#include "../pins.h"

StateScreenOff::StateScreenOff() {
  _adc_saved_state = 0;
}

void StateScreenOff::disable_ADC() {
    _adc_saved_state = ADCSRA;
    ADCSRA = 0; // disable ADC
}

void StateScreenOff::enable_ADC() {
  if (_adc_saved_state != 0) {
    ADCSRA = _adc_saved_state;
  }
}

// interuption handler
void wake_up() {
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(PIN_BT1));
  detachInterrupt(digitalPinToInterrupt(PIN_BT2));
}

void StateScreenOff::on_enter(Context* context) {
  context->hpdl1414->set_message("    ");
  context->hpdl1414->set_luminosity(0);

  // switch off VCC on external components
  context->vccController->switch_power_off();

  // disconnect all HPDL1414 pins
  context->hpdl1414->switch_off();

  // disable ADC (analog to digital conversion)
  disable_ADC();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // timed sequence follows
  noInterrupts();

  attachInterrupt(digitalPinToInterrupt(PIN_BT1), wake_up, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BT2), wake_up, FALLING);
  EIFR = bit(INTF0) | bit(INTF1);  // clear flag for interrupt 0 & 1

  sleep_enable();

  // turn off brown-out enable in software
  // BODS must be set to one and BODSE must be set to zero within four clock cycles
  MCUCR = bit (BODS) | bit (BODSE);
  // The BODS bit is automatically cleared after three clock cycles
  MCUCR = bit (BODS);

  // guarantees next instruction executed
  interrupts();
  // sleep within 3 clock cycles of above
  sleep_cpu();
}

void StateScreenOff::in_loop(Context* context) {
  if (context->bt1->is_pressed_event() || context->bt2->is_pressed_event()) {
    Configuration configuration = context->persist->read();
    context->state_manager->transition_to(StateCode::STATE_SHOW_TIME, context, configuration.switch_on_transition);
  }
}

void StateScreenOff::on_exit(Context* context) {
  // context->hpdl1414->fade_in(context->persist->read().prefered_luminosity);
  context->hpdl1414->switch_on();
  context->vccController->switch_power_on();
  enable_ADC();
}
