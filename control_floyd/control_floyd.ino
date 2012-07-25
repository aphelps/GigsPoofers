//#define DEBUG
//#define DEBUG_VERBOSE 2

#include "Debug.h"
#include "State.h"

#define NUM_POOFERS 1

State state(NUM_POOFERS);

void setup() {
  Serial.begin(9600);

}


unsigned int ontime = millis();

#define NUM_MODES 3
unsigned int mode = 0;

void loop() {
  mode = (millis() / 1024) % NUM_MODES;

  for (int i = 0; i < NUM_POOFERS; i++) {
    switch (mode) {
        case 0:
          if (!state.get_ign(i)) state.set_ign(i, true);
          if (state.get_poof(i)) state.set_poof(i, false);
          break;
        case 1:
          if (state.get_ign(i)) state.set_ign(i, false);
          if (!state.get_poof(i)) state.set_poof(i, true);
          break;
        case 2:
          if (state.get_ign(i)) state.set_ign(i, false);
          if (state.get_poof(i)) state.set_poof(i, false);
          break;
    }
  }
  
  state.transmit();
  delay(10);
}
