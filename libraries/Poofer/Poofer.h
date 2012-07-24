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
         boolean default_open);
  void setIgnTimeout(unsigned long ign_max_ms);
  void setSolTimeout(unsigned long sol_max_ms);

  /*
   * Examine the sensor states and set the output values appropriately
   */
  void processState(void);

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
