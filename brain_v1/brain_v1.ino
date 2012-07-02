#include "Pins.h"
#include "Debug.h"

/* Relay pins */
#define SOLENOID_1_RELAY_PIN 4
#define IGNITER_1_RELAY_PIN 3
#define SOLENOID_2_RELAY_PIN 2
#define IGNITER_2_RELAY_PIN 1

/* Arduino pins */
int SOLENOID_1_LED_PIN = 10;
int IGNITER_1_LED_PIN = 9;
int SOLENOID_2_LED_PIN = 6;
int IGNITER_2_LED_PIN = 5;

int SOLENOID_1_SWITCH_PIN = 13;
int IGNITER_1_SWITCH_PIN = 2;
int SOLENOID_2_SWITCH_PIN = 3;
int INGNITER_2_SWITCH_PIN = 4;

Switch poofer_solenoid_1(7, false, switch_solenoid, (void *)SOLENOID_1_LED_PIN);
Switch poofer_igniter_1(8,  false, switch_igniter,  (void *)IGNITER_1_LED_PIN);
Switch poofer_solenoid_2(11,  false, switch_solenoid, (void *)SOLENOID_2_LED_PIN);
Switch poofer_igniter_2(12,   false, switch_igniter,  (void *)IGNITER_2_LED_PIN);

#define NUM_PINS 21
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,               // D0: RX - to Xbee
  NULL,               // D1: TX - to Xbee
  NULL,               // D2: Igniter 1 -> Relay board IN 3
  NULL,               // D3: Solenoid 2 -> Relay board IN 2
  NULL,               // D4: Igniter 2 -> Relay board IN 1 
  NULL,               // D5: Igniter 2 on light 
  NULL,               // D6: Solenoid 2 on light
  &poofer_solenoid_1,  // D7: Solenoid 1 X
  &poofer_igniter_1, // D8: Igniter 1 X
  NULL,               // D9: Igniter 1 on light
  NULL,               // D10: Solenoid 1 on light
  &poofer_solenoid_2,  // D11: Solenoid 2 X
  &poofer_igniter_2, // D12: Igniter 2 X
  NULL,               // D13: Solenoid 1 -> Relay board IN 4

  /* Analog Pins */
  NULL,               // A1: Empty
  NULL,               // A2: Empty
  NULL,               // A3: Empty
  NULL,               // A4: Empty
  NULL,               // A5: Empty
  NULL,               // A6: Empty
  NULL,               // A7: Empty
};


void setup() {
  Serial.begin(9600);

  pinMode(SOLENOID_1_LED_PIN, OUTPUT);
  pinMode(IGNITER_1_LED_PIN, OUTPUT);
  pinMode(SOLENOID_2_LED_PIN, OUTPUT);
  pinMode(IGNITER_2_LED_PIN, OUTPUT);
}

void loop() {

  /* Check switches and perform actions */
  checkSwitches(pinArray, NUM_PINS, true);

  delay(10);
}

