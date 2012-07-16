#define IGNITION_TIMEOUT_MS 5000
#define SOLENOID_TIMEOUT_MS 500

class Poofer
{
  public:
  int sol_sensor;
  int sol_led;
  int sol_relay;
  int ign_sensor;
  int ign_led;
  int ign_relay;

  int sol_value;
  unsigned long sol_turned_on_ms;
  
  int ign_value;
  unsigned long ign_turned_on_ms;

  public:
  Poofer(int _sol_sensor,
         int _sol_led,
         int _sol_relay,
         int _ign_sensor,
         int _ign_led,
         int _ign_relay) {
    sol_sensor = _sol_sensor;
    sol_led = _sol_led;
    sol_relay = _sol_relay;
    ign_sensor = _ign_sensor;
    ign_led = _ign_led;
    ign_relay = _ign_relay;

    ign_value = 0;
    ign_turned_on_ms = 0;

    sol_value = 0;
    sol_turned_on_ms = 0;

    /* Set the output pins */
    pinMode(sol_led, OUTPUT);
    pinMode(sol_relay, OUTPUT);
    pinMode(ign_led, OUTPUT);
    pinMode(ign_relay, OUTPUT);

    /* Initialize the outputs to LOW */
    digitalWrite(sol_led, LOW);
    digitalWrite(sol_relay, LOW);
    digitalWrite(ign_led, LOW);
    digitalWrite(ign_relay, LOW);
  };

};
