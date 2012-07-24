/*
 * A class to handle processing of the sensors and signals related to a poofer
 * and to set the outputs appropriately.
 */
#include "Pins.h"
#include "State.h"

#define IGNITION_TIMEOUT_MS 5000
#define SOLENOID_TIMEOUT_MS 2000

class Poofer
{
public:
  Poofer(int id, State *state,
         Sensor *ign_switch, Output *ign_relay, Output *ign_led,
         Sensor *sol_switch, Output *sol_relay, Output *sol_led,
         boolean default_open) {
    _id = id;
    _state = state;

    _ign_switch = ign_switch;
    _ign_relay  = ign_relay;
    _ign_led    = ign_led;

    _sol_switch = sol_switch;
    _sol_relay  = sol_relay;
    _sol_led    = sol_led;

    _ign_on = _sol_on = false;
    _ign_on_ms = _sol_on_ms = 0;

    _ign_max_ms = IGNITION_TIMEOUT_MS;
    _sol_max_ms = SOLENOID_TIMEOUT_MS;

    if (default_open) {
      /*
       * Default open indicates that the relay is open unless triggered,
       * which would mean on even when the Arduino is powered down.  Set
       * to HIGH to start off as closed.
       */
      _on_value = LOW;
      _off_value = HIGH;
    } else {
      _on_value = HIGH;
      _off_value = LOW;
    }
    _ign_relay->setValue(_off_value);
    _sol_relay->setValue(_off_value);
  }

  /*
   * Examine the switch states and set the output values appropriately
   */
  void processState(void) {
    unsigned long now = millis();

    if (_ign_on) {
      // Disable the ignitor when:
      if (((_ign_switch->curr_state == LOW) &&
           (_state->get_ign(_id)) == LOW) ||   // Switch and xbee state is off
          ((now - _ign_on_ms) > _ign_max_ms)   // Timeout has elapsed
          ) {
        /* Disable when switch is turned off or timer elapses */
        _ign_on = false;
        _ign_relay->setValue(_off_value);
        _ign_led->setValue(LOW);
      }
    } else {
      // Enable the igniter when
      if ((_ign_switch->curr_state == HIGH) ||
          (_state->get_ign(_id) == HIGH)       // Switch or xbee state is on
          ) {
        if (_ign_on_ms == 0) {
          /* Switch was turned on while in a reset set */
          _ign_on = true;
          _ign_relay->setValue(_on_value);
          _ign_led->setValue(HIGH);
          _ign_on_ms = now;
        }
      } else {
        /* Reset when switch goes to low */
        _ign_on_ms = 0;
      }
    }

    if (_sol_on) {
      /* Disable the solenoid when : */
      if ((_sol_switch->curr_state == LOW) ||   // Switch is off 
          _ign_on ||                            // Ignitor is on
          ((now - _sol_on_ms) > _sol_max_ms)    // Timeout has elapsed
          ) { 
        _sol_on = false;
        _sol_relay->setValue(_off_value);
        _sol_led->setValue(LOW);
      }
    } else {
      if (_sol_switch->curr_state == HIGH) {
        if ((_sol_on_ms == 0) && (!_ign_on)) {
          /* Switch was turned on while in a reset set */
          _sol_on = true;
          _sol_relay->setValue(_on_value);
          _sol_led->setValue(HIGH);
          _sol_on_ms = now;
        }
      } else {
        /* Reset when switch goes to low */
        _sol_on_ms = 0;
      }
    }

  }

private:
  int _id;
  State *_state;

  Sensor *_ign_switch, *_sol_switch;
  Output *_ign_relay, *_ign_led, *_sol_relay, *_sol_led;

  boolean _ign_on, _sol_on;
  boolean _on_value, _off_value;
  unsigned long _ign_max_ms, _sol_max_ms;
  unsigned long _ign_on_ms, _sol_on_ms;
};
