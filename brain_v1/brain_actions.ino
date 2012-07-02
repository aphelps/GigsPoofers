void switch_solenoid(int pin, int value, void *arg)
{
  int led_pin = (int)arg;
  digitalWrite(led_pin, value);
  DEBUG_PRINT(2, "switch_solenoid pin=");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " led_pin=");
  DEBUG_PRINT(2, led_pin);
  DEBUG_PRINT(2, " value=");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, "\n");
}


void switch_igniter(int pin, int value, void *arg)
{
  int led_pin = (int)arg;
  digitalWrite(led_pin, value);
  DEBUG_PRINT(2, "switch_igniter pin=");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " led_pin=");
  DEBUG_PRINT(2, led_pin);
  DEBUG_PRINT(2, " value=");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, "\n");
}


