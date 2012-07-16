#include <State.h>

#define NUM_POOFERS 2

State state(NUM_POOFERS);

int led_pins[2][2] = {{4,5},{6,7}};
int poof_pins[2][2] = {{8,9},{10,11}};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_POOFERS; i++) {
    for (int j = 0; j < 2; j++) {
      pinMode(led_pins[i][j], OUTPUT);
      pinMode(poof_pins[i][j], OUTPUT);    }
  }
}

#define OFF_VALUE_MS (unsigned long)-1

#define IGN_MAX_MS 15000
unsigned long ign_started_ms[NUM_POOFERS];

#define POOF_MAX_MS 2000
unsigned long poof_started_ms[NUM_POOFERS];

void loop() {
  state.receive();

  for (int i = 0; i < NUM_POOFERS; i++) {

    if (state.get_ign(i)) {
      Serial.print("Ign:");
      Serial.print(i);
      
#if 1
      Serial.print(" ign_ms:");
      Serial.print(ign_started_ms[i]);
      Serial.print(" millis:");
      Serial.print(millis()); 
      if ((ign_started_ms[i] == 0) || (millis() - ign_started_ms[i] < IGN_MAX_MS)) {
        if (ign_started_ms[i] == 0) {
          ign_started_ms[i] = millis();
        }
        digitalWrite(led_pins[i][0], HIGH);
        digitalWrite(poof_pins[i][0], HIGH);
      } else {
        digitalWrite(led_pins[i][0], LOW);
        digitalWrite(poof_pins[i][0], LOW);
      }
#else
     digitalWrite(led_pins[i][0], HIGH);
     digitalWrite(poof_pins[i][0], HIGH);
#endif
    } else {
      ign_started_ms[i] = 0;
      digitalWrite(led_pins[i][0], LOW);
      digitalWrite(poof_pins[i][0], LOW);
    }
    
    if (state.get_poof(i)) {
      Serial.print("Poof ");
      Serial.print(i);
      
#if 1
      Serial.print(" poof_ms:");
      Serial.print(poof_started_ms[i]);
      Serial.print(" millis:");
      Serial.print(millis()); 
      if ((poof_started_ms[i] == 0) || (millis() - poof_started_ms[i] < POOF_MAX_MS)) {
        if (poof_started_ms[i] == 0) {
          poof_started_ms[i] = millis();
        }
        digitalWrite(led_pins[i][1], HIGH);
        digitalWrite(poof_pins[i][1], HIGH);
      } else {
        digitalWrite(led_pins[i][1], LOW);
        digitalWrite(poof_pins[i][1], LOW);
      }
#else
     digitalWrite(led_pins[i][1], HIGH);
     digitalWrite(poof_pins[i][1], HIGH);
#endif
    } else {
      poof_started_ms[i] = 0;
      digitalWrite(led_pins[i][1], LOW);      
      digitalWrite(poof_pins[i][1], LOW);
    }
    
    Serial.println();
  }
}
