

#include <State.h>
#include <PooferControl.h>

#define NUM_POOFERS 2

State state(NUM_POOFERS);

PooferControl controls[NUM_POOFERS] = {
  PooferControl(0, 2, 3, 4, 5, 6, state),
  PooferControl(1, 7, 8, 9, 10, 11, state)};
  
void setup() {
  Serial.begin(9600);

}

void loop() {
  for (int i = 0; i < NUM_POOFERS; i++) {
    controls[i].updateState();
  }
  
  state.transmit();
  delay(10);
}
