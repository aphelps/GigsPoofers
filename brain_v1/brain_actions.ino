#if 0
void switch_solenoid(int pin, int value, void *arg)
{
  Poofer *poofer = (Poofer *)arg;
  digitalWrite(poofer->sol_led, value);
  digitalWrite(poofer->sol_relay, value);
  DEBUG_PRINT(2, "switch_solenoid pin=");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " sol_relay=");
  DEBUG_PRINT(2, poofer->sol_relay);
  DEBUG_PRINT(2, " sol_led=");
  DEBUG_PRINT(2, poofer->sol_led);
  DEBUG_PRINT(2, " value=");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, "\n");
}
#endif
void switch_solenoid(int pin, int value, void *arg)
{
  Poofer *poofer = (Poofer *)arg;

  poofer->sol_value = 0;
  if (value) {
    if ((poofer->sol_turned_on_ms == 0) ||
      millis() < poofer->sol_turned_on_ms + SOLENOID_TIMEOUT_MS) {
      if (poofer->sol_turned_on_ms == 0)
        poofer->sol_turned_on_ms = millis();
      poofer->sol_value = 1;
    }
  } else {
    poofer->sol_turned_on_ms = 0;
  }

  digitalWrite(poofer->sol_led, poofer->sol_value);
  digitalWrite(poofer->sol_relay, poofer->sol_value);
  DEBUG_PRINT(2, "switch_solenoid pin=");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " value=");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, " sol_relay=");
  DEBUG_PRINT(2, poofer->sol_relay);
  DEBUG_PRINT(2, " sol_led=");
  DEBUG_PRINT(2, poofer->sol_led);
  DEBUG_PRINT(2, " sol_value=");
  DEBUG_PRINT(2, poofer->sol_value);
  DEBUG_PRINT(2, " sol_turned_on_ms=");
  DEBUG_PRINT(2, poofer->sol_turned_on_ms);
  DEBUG_PRINT(2, "\n");

}



void switch_igniter(int pin, int value, void *arg)
{
  Poofer *poofer = (Poofer *)arg;

  poofer->ign_value = 0;
  if (value) {
    if ((poofer->ign_turned_on_ms == 0) ||
      millis() < poofer->ign_turned_on_ms + IGNITION_TIMEOUT_MS) {
      if (poofer->ign_turned_on_ms == 0)
        poofer->ign_turned_on_ms = millis();
      poofer->ign_value = 1;
    }
  } else {
    poofer->ign_turned_on_ms = 0;
  }

  digitalWrite(poofer->ign_led, poofer->ign_value);
  digitalWrite(poofer->ign_relay, poofer->ign_value);
  DEBUG_PRINT(2, "switch_igniter pin=");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " value=");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, " ign_relay=");
  DEBUG_PRINT(2, poofer->ign_relay);
  DEBUG_PRINT(2, " ign_led=");
  DEBUG_PRINT(2, poofer->ign_led);
  DEBUG_PRINT(2, " ign_value=");
  DEBUG_PRINT(2, poofer->ign_value);
  DEBUG_PRINT(2, " ign_turned_on_ms=");
  DEBUG_PRINT(2, poofer->ign_turned_on_ms);
  DEBUG_PRINT(2, "\n");

}


