#include "StateManager.h"

#include "../StateScreenOff.h"
#include "../StateShowTime.h"
#include "../StateShowDate.h"
#include "../StateShowTemperature.h"
#include "../StateShowSeconds.h"
#include "../StateShowVoltage.h"
#include "../StateSetupYear.h"
#include "../StateSetupMonth.h"
#include "../StateSetupDay.h"
#include "../StateSetupHours.h"
#include "../StateSetupMins.h"
#include "../StateSetupLuminosity.h"
#include "../StateSetupSwitchOnText.h"
#include "../StateSetupSwitchOnTransition.h"
#include "../transitions/RandomFadeIn.h"
#include "../transitions/FadeOut.h"
#include "../transitions/TextFadeIn.h"
#include "../transitions/TextSlide.h"
#include "../transitions/TimeFadeIn.h"

StateManager::StateManager(
) : _current_state(NULL),
    _transition_state(NULL),
    _next_state(NULL)
{}

void StateManager::in_loop(Context* context) {
  if (_next_state != NULL) {
    if (_current_state != NULL) {
      _current_state->on_exit(context);
      delete _current_state;
      _current_state = NULL;
      if (_transition_state != NULL) {
        _transition_state->on_enter(context);
      }
    } else if (_transition_state != NULL) {
      if (_transition_state->finished(context)) {
        _transition_state->on_exit(context);
        delete _transition_state;
        _transition_state = NULL;
      } else {
        _transition_state->in_loop(context); 
      }
    } else {
      _current_state = _next_state;
      _next_state = NULL;
      _current_state->on_enter(context);
    }
  } else if (_current_state != NULL) {
    _current_state->in_loop(context);
  }
}

void StateManager::transition_to(StateCode new_state_code, Context* context, TransitionCode transition_code ) {
  if (transition_code == TransitionCode::TRANSITION_RANDOM_FADE_IN) {
    _transition_state = new RandomFadeIn();
  } else if (transition_code == TransitionCode::TRANSITION_FADE_OUT) {
    _transition_state = new FadeOut();
  } else if (transition_code == TransitionCode::TRANSITION_TEXT_FADE_IN) {
    _transition_state = new TextFadeIn();
  } else if (transition_code == TransitionCode::TRANSITION_TEXT_SLIDE) {
    _transition_state = new TextSlide();
  } else if (transition_code == TransitionCode::TRANSITION_TIME_FADE_IN) {
    _transition_state = new TimeFadeIn();
  } else { // TransitionCode::TRANSITION_NONE
    _transition_state = NULL;
  }

  if (new_state_code == StateCode::STATE_SCREEN_OFF) {
    _next_state = new StateScreenOff();
  } else if (new_state_code == StateCode::STATE_SHOW_TIME) {
    _next_state = new StateShowTime();
  } else if (new_state_code == StateCode::STATE_SHOW_DATE) {
    _next_state = new StateShowDate();
  } else if (new_state_code == StateCode::STATE_SHOW_TEMPERATURE) {
    _next_state = new StateShowTemperature();
  } else if (new_state_code == StateCode::STATE_SHOW_SECONDS) {
    _next_state = new StateShowSeconds();
  } else if (new_state_code == StateCode::STATE_SHOW_VOLTAGE) {
    _next_state = new StateShowVoltage();
  } else if (new_state_code == StateCode::STATE_SETUP_YEAR) {
    _next_state = new StateSetupYear();
  } else if (new_state_code == StateCode::STATE_SETUP_MONTH) {
    _next_state = new StateSetupMonth();
  } else if (new_state_code == StateCode::STATE_SETUP_DAY) {
    _next_state = new StateSetupDay();
  } else if (new_state_code == StateCode::STATE_SETUP_HOURS) {
    _next_state = new StateSetupHours();
  } else if (new_state_code == StateCode::STATE_SETUP_MINS) {
    _next_state = new StateSetupMins();
  } else if (new_state_code == StateCode::STATE_SETUP_LUMINOSITY) {
    _next_state = new StateSetupLuminosity();
  } else if (new_state_code == StateCode::STATE_SETUP_SWITCH_ON_TRANSITION) {
    _next_state = new StateSetupSwitchOnTransition();
  } else if (new_state_code == StateCode::STATE_SETUP_SWITCH_ON_TEXT) {
    _next_state = new StateSetupSwitchOnText();
  }
}

void StateManager::transition_to(StateCode new_state_code, Context* context) {
  transition_to(new_state_code, context, TransitionCode::TRANSITION_NONE);
}
