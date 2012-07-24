#include "Debug.h"
#include "State.h"

#define NUM_POOFERS 1

State state(NUM_POOFERS);

void setup() {
  Serial.begin(9600);

}


unsigned int ontime = 0;

void loop() {
  for (int i = 0; i < NUM_POOFERS; i++) {
    if ((ontime == 0) || ((millis() - ontime) < 1000)) {
      state.set_ign(0, true);
      state.set_poof(0, true);
      if (ontime == 0) ontime = millis();
    } else {
      state.set_ign(0, false);
      state.set_poof(0, false);
      if (millis() - ontime > 2000) {
        ontime = 0;
      }
    }
  }
  
  state.transmit();
  delay(10);
}
