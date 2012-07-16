#include <State.h>

#define NUM_POOFERS 2

State state(NUM_POOFERS);

int pins[2][2] = {{4,5},{6,7}};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_POOFERS; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(pins[i][j], OUTPUT);
    }
  }
}

void loop() {
  state.receive();
  for (int i = 0; i < NUM_POOFERS; i++) {
    if (state.get_ign(i)) {
      digitalWrite(pins[i][0], HIGH);
    } else {
      digitalWrite(pins[i][0], LOW);
    }
    
    if (state.get_poof(i)) {
      digitalWrite(pins[i][1], HIGH);
    } else {
      digitalWrite(pins[i][1], LOW);
    }
  }
}
