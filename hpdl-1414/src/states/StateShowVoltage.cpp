
#include "StateShowVoltage.h"

StateShowVoltage::StateShowVoltage(
) : MixinTimeoutToScreenOff(5000)
{
  _vcc_millivolts = 0;
  _last_read_time_ms = 0;
}

uint16_t readVccMillivolts() {
  // Set up ADC to read internal 1.1V reference
  // MUX[3:0]=1110, REFS0=1
  ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
  // Let Vref settle
  delay(2);
  // Start conversion and wait until done.
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));  
  uint16_t result = ADC;
  // 1.1V * 1023 * 1000 (to get mV)
  return (uint16_t)(1125300L / result);
}

// reads a more stable value than the original.
uint16_t readVccMillivoltsAverage(uint8_t samples) {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += readVccMillivolts();
  }
  return (uint16_t)(sum / samples);
}

uint8_t voltageToPercentage(uint16_t voltage) {
  // Clamp voltage to expected range
  if (voltage >= 3000) {
    return 100;
  } else if (voltage <= 1800) {
    return 0;
  }

  // Piecewise linear approximation
  if (voltage > 2900) {
    return min(100, 95 + (voltage - 2900) / 20);  // 2.9–3.0V → 95–100%
  } else if (voltage > 2700) {
    return 75 + (voltage - 2700) / 10;  // 2.7–2.9V → 75–95%
  } else if (voltage > 2200) {
    return 25 + (voltage - 2200) / 10;  // 2.2–2.7V → 25–75%
  } else {
    return max(0, (voltage - 1800) / 16);  // 1.8–2.2V → 0–25%
  }
}

void StateShowVoltage::on_enter(Context* context) {
  MixinTimeoutToScreenOff::on_enter(context);
  if (context->bt2->is_pressed()) {
    context->bt2->ignore_next_released_event();
  }
  if (context->bt1->is_pressed()) {
    context->bt1->ignore_next_released_event();
  }
}

void StateShowVoltage::in_loop(Context* context) {
  MixinTimeoutToScreenOff::in_loop(context);

  uint32_t time = millis();
  if (_last_read_time_ms == 0 || _last_read_time_ms + 1000 < time) {
    _last_read_time_ms = time;
    _vcc_millivolts = readVccMillivoltsAverage(10);
  }

  char the_string[5];
  if (context->persist->read().voltage_in_percentage) {
    uint8_t percentage = voltageToPercentage(_vcc_millivolts);
    snprintf(
      the_string,
      sizeof(the_string),
      "%3u%%",
      percentage
    );
  } else {
    snprintf(
      the_string,
      sizeof(the_string),
      "%1u.%1uV",
      _vcc_millivolts / 1000,
      (_vcc_millivolts % 1000) / 100
    );
  }

  context->hpdl1414->set_message(the_string);

  if (context->bt1->is_released_event()) {
    Configuration conf = context->persist->read();
    conf.voltage_in_percentage = ! conf.voltage_in_percentage;
    context->persist->save(conf);
  }
  if (context->bt2->is_released_event()) {
    context->state_manager->transition_to(StateCode::STATE_SHOW_TIME, context);
  }
}

void StateShowVoltage::on_exit(Context* context) {
  MixinTimeoutToScreenOff::on_exit(context);
}