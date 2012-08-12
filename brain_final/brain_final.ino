/******************************************************************************
 * The final brain box for Gigsville Poofers in 2012
 *
 * This has a line of 18 screw terminals connected to various things.
 *
 * Screw  Nano/Xbee Wire
 *   S1:  D13       ->  XXX - For some reason this doesn't work as a switch (pull-up resistor or wiring problem?)
 *   S2:  D12       -> Shift register - Clock bin (Blue)
 *   S3:  D11       -> Shift register - Data pin (White)
 *   S4:  D10       -> Shift register - Latch pin (Red)
 *   S5:  D9        -> Switch - Ign 1
 *   S6:  D8        -> Switch - Sol 1
 *   S7:  D7        -> Switch - Ign 2
 *   S8:  D6        -> Switch - Ign 2
 *   S9:  D5        -> Relay - Ign 1 & LED
 *   S10: D4        -> Relay - Sol 1 & LED
 *   S11: D3        -> Relay - Ign 2 & LED
 *   S12: D2        -> Relay - Sol 2 & LED
 *   S13: Ground    -> 
 *   S14: Ground    -> 
 *   S15: 5V        -> 
 *   S16: 5V        ->
 *   S17: RX0       -> Xbee switch
 *   S18: Xbee Dout -> Xbee switch
 *
 *   SA:  A5        -> Serial LCD
 *   SB:  A4        -> Serial LCD
 *   SC:  A0
 * *** Shift register will take 3 outputs?
 *
 * Pan ID: 47494753 "GIGS"
 * XBee Pro: Coordinator API
 *   SL: 40982416
 *
 * Xbee Pro: Router API
 *   SL: 4098240E
 *
 * Xbee: Router API
 *   SL: 40315516
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
