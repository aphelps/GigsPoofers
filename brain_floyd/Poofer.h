/*
 * A class to handle processing of the sensors and signals related to a poofer
 * and to set the outputs appropriately.
 */
#include "Pins.h"

#define IGNITION_TIMEOUT_MS 5000
#define SOLENOID_TIMEOUT_MS 2000

class Poofer
{
public:
  Poofer(Sensor *ign_switch, Output *ign_relay, Output *ign_led,
         Sensor *sol_switch, Output *sol_relay, Output *sol_led) {  
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
  }

  /*
   * Examine the switch states and set the output values appropriately
   */
  void processState(void) {
    unsigned long now = millis();

    if (_ign_on) {
      if ((_ign_switch->curr_state == LOW) ||
          ((now - _ign_on_ms) > _ign_max_ms)) {
        /* Disable when switch is turned off or timer elapses */
        _ign_on = false;
        _ign_relay->setValue(LOW);
        _ign_led->setValue(LOW);
      }
    } else {
      if (_ign_switch->curr_state == HIGH) {
        if (_ign_on_ms == 0) {
          /* Switch was turned on while in a reset set */
          _ign_on = true;
          _ign_relay->setValue(HIGH);
          _ign_led->setValue(HIGH);
          _ign_on_ms = now;
        }
      } else {
        /* Reset when switch goes to low */
        _ign_on_ms = 0;
      }
    }

    if (_sol_on) {
      if ((_sol_switch->curr_state == LOW) ||
          ((now - _sol_on_ms) > _sol_max_ms)) {
        /* Disable when switch is turned off or timer elapses */
        _sol_on = false;
        _sol_relay->setValue(LOW);
        _sol_led->setValue(LOW);
      }
    } else {
      if (_sol_switch->curr_state == HIGH) {
        if (_sol_on_ms == 0) {
          /* Switch was turned on while in a reset set */
          _sol_on = true;
          _sol_relay->setValue(HIGH);
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
  Sensor *_ign_switch, *_sol_switch;
  Output *_ign_relay, *_ign_led, *_sol_relay, *_sol_led;

  boolean _ign_on, _sol_on;
  unsigned long _ign_max_ms, _sol_max_ms;
  unsigned long _ign_on_ms, _sol_on_ms;
};
