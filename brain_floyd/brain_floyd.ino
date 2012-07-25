/******************************************************************************
 * This controls the version of the protoboard I made in Floyd.
 *
 * This has a line of 18 screw terminals connected to various things.  I made
 * a mistake with the pin-terminal wiring which is being corrected in software.
 *
 * Screw  Nano/Xbee Wire
 *   S1:  D13       -> Igniter LED XXX - For some reason this doesn't work as a switch (pull-up resistor or wiring problem?)
 *   S2:  D11       -> Valve LED
 *   S3:  D10       -> Igniter switch
 *   S4:  D9        -> Valve switch
 *   S5:  D8        -> LED
 *   S6:  D12       -> LED
 *   S7:  D7        -> LED
 *   S8:  D6        -> LED
 *   S9:  D5        -> LED
 *   S10: D4        -> LED
 *   S11: D3        -> Igniter relay
 *   S12: D2        -> Valve relay
 *   S13: Ground    -> LEDs + Switch
 *   S14: Ground    -> Relay
 *   S15: 5V        -> Relay + Power LED
 *   S16: RX0       -> Xbee switch
 *   S17: Xbee Dout -> Xbee switch
 *   S18:
 *
 * Analog pin A0 is also attached to a photoresistor
 *****************************************************************************/

//#define DEBUG
//#define DEBUG_VERBOSE 2

#include "Debug.h"
#include "Pins.h"
#include "Poofer.h"
#include "State.h"

#define VALVE_RELAY_PIN    2
#define VALVE_SWITCH_PIN   9
#define VALVE_LED_PIN      11

#define IGNITER_RELAY_PIN  3
#define IGNITER_SWITCH_PIN 10
#define IGNITER_LED_PIN    13

#define LED1_PIN 4
#define LED2_PIN 5
#define LED3_PIN 6
#define LED4_PIN 7
#define LED5_PIN 12
#define LED6_PIN 8

Output valve_led(VALVE_LED_PIN, LOW);
Output valve_relay(VALVE_RELAY_PIN, LOW);
Output igniter_led(IGNITER_LED_PIN, LOW);
Output igniter_relay(IGNITER_RELAY_PIN, LOW);

Sensor valve_switch(VALVE_SWITCH_PIN, true, false, NULL);
Sensor igniter_switch(IGNITER_SWITCH_PIN, true, false, NULL);
Sensor photoresistor(A0, true, true, action_print_value);

Output LED1(LED1_PIN, LOW);
Output LED2(LED2_PIN, LOW);
Output LED3(LED3_PIN, LOW);
Output LED4(LED4_PIN, LOW);
Output LED5(LED5_PIN, LOW);
Output LED6(LED6_PIN, LOW);

#define NUM_PINS 15 // Digital pins only
// #define NUM_PINS 21 // Digital + analog
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,               // D0: RX - to Xbee
  NULL,               // D1: TX - to Xbee
  &valve_relay,       // D2: Valve relay
  &igniter_relay,     // D3: Igniter relay
  &LED1,              // D4: LED
  &LED2,              // D5: LED
  &LED3,              // D6: LED
  &LED4,              // D7: LED
  &LED5,              // D8: LED
  &valve_switch,      // D9: Valve switch
  &igniter_switch,    // D10: Igniter switch
  &valve_led,         // D11: Valve LED
  &LED6,              // D12: LED
  &igniter_led,       // D13: Igniter LED

  /* Analog Pins */
  &photoresistor,       // A0: Photo Resistor
#if 0
  NULL,               // A1: Empty
  NULL,               // A2: Empty
  NULL,               // A3: Empty
  NULL,               // A4: Empty
  NULL,               // A5: Empty
  NULL,               // A6: Empty
  NULL,               // A7: Empty
#endif
};

#define NUM_POOFERS 1
State state(NUM_POOFERS);

Poofer poofers[NUM_POOFERS] = {
  Poofer(0, &state,&igniter_switch, &igniter_relay, &igniter_led,
         &valve_switch, &valve_relay, &valve_led, true),
};

void setup() {
  Serial.begin(9600);
}

void loop() {

  /* Check sensores and perform actions */
  checkSensors(pinArray, NUM_PINS, false);

  /* Receive commands via XBee */
  state.receive();

  /* Based on the inputs determined what to do with the poofers */
  for (int i = 0; i < NUM_POOFERS; i++) {
    poofers[i].processState();
  }

  /* Trigger the outputs */
  triggerOutputs(pinArray, NUM_PINS);

  DEBUG_COMMAND(XXXdelay(100));
}
