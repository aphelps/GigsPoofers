/*
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
 */

//#define DEBUG
//#define DEBUG_VERBOSE 2

#include "Pins.h"
#include "Debug.h"

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

Output valve_LED(VALVE_LED_PIN, LOW);
Output valve_relay(VALVE_RELAY_PIN, LOW);
Output igniter_LED(IGNITER_LED_PIN, LOW);
Output igniter_relay(IGNITER_RELAY_PIN, LOW);

Output LED1(LED1_PIN, HIGH);
Output LED2(LED2_PIN, LOW);
Output LED3(LED3_PIN, HIGH);
Output LED4(LED4_PIN, LOW);
Output LED5(LED5_PIN, HIGH);
Output LED6(LED6_PIN, LOW);

Sensor valve_switch(VALVE_SWITCH_PIN, true, false,
                    action_set_output, (void *)&valve_LED);
Sensor igniter_switch(IGNITER_SWITCH_PIN, true, false,
                      action_set_output, (void *)&igniter_LED);

#define NUM_PINS 14 // Digital pins only
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
  &valve_LED,         // D11: Valve LED
  &LED6,              // D12: LED
  &igniter_LED,       // D13: Igniter LED

#if 0
  /* Analog Pins */
  NULL,               // A1: Empty
  NULL,               // A2: Empty
  NULL,               // A3: Empty
  NULL,               // A4: Empty
  NULL,               // A5: Empty
  NULL,               // A6: Empty
  NULL,               // A7: Empty
#endif
};


void setup() {
  Serial.begin(9600);
}

void loop() {

  /* Check sensores and perform actions */
  checkSensors(pinArray, NUM_PINS, false);

  /* Receive commands via XBee */
  // XXX - Todo

  /* Based on the inputs determined what to do with the poofers */
  // XXX - Todo

  /* Trigger the outputs */
  triggerOutputs(pinArray, NUM_PINS);

  DEBUG_COMMAND(delay(100));
}



/* Actions */

/* Set an output's next value to the value of the sensor */
void action_set_output(int pin, int value, void *arg) 
{
  Output *out = (Output *)arg;

  out->setValue(value);
  DEBUG_PRINT(2, "set_output: pin-");
  DEBUG_PRINT(2, pin);
  DEBUG_PRINT(2, " value-");
  DEBUG_PRINT(2, value);
  DEBUG_PRINT(2, " outpin-");
  DEBUG_PRINT(2, out->_value);
  DEBUG_PRINT(2, "\n");
}