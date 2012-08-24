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
 *   S8:  D6        -> Switch - Sol 2
 *   S9:  D5        -> 
 *   S10: D4        -> 
 *   S11: D3        -> 
 *   S12: D2        -> 
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

 * Shift register (74HC595) outputs:
 *   0  -> Relay 1 - Solenoid 1
 *   1  -> Relay 2 - Ignitor 1
 *   2  -> Relay 3 - Solenoid 2
 *   3  -> Relay 4 - Ignitor 2
 *   4  -> Relay 5
 *   5  -> Relay 6
 *   6  -> Relay 7
 *   7  -> Relay 8
 *   8  -> Big Relay
 *   9  -> LED
 *   10 -> LED
 *   11 -> LED
 *   12 -> LED
 *   13 -> Empty
 *   14 -> Empty
 *   15 -> Empty
 *
 * Pan ID: 4747 "GG"
 * XBee Pro: Coordinator API
 *   SL: 40982416
 *
 * Xbee Pro: Router API
 *   SL: 4098240E
 *
 * Xbee: Router API
 *   SL: 40315516
 *****************************************************************************/

#define DEBUG
#define DEBUG_VERBOSE 2

#include "Debug.h"
#include "LiquidCrystal.h"
#include "Pins.h"
#include "Poofer.h"
#include "Shift.h"
#include "State.h"
#include "Wire.h"

/* Pins */
#define VALVE_2_SWITCH   6
#define IGNITER_2_SWITCH 7
#define VALVE_1_SWITCH   8
#define IGNITER_1_SWITCH 9
#define SHIFT_LATCH      10 // Shift register
#define SHIFT_DATA       11 // Shift register
#define SHIFT_CLOCK      12 // Shift register


/* Shift register outputs */
#define SHIFT_RELAY_1 0
#define SHIFT_RELAY_2 1
#define SHIFT_RELAY_3 2
#define SHIFT_RELAY_4 3
#define SHIFT_RELAY_5 4 /* Igniter 1? */
#define SHIFT_RELAY_6 5 /* Valve 1? */
#define SHIFT_RELAY_7 6 /* Igniter 2? */
#define SHIFT_RELAY_8 7 /* Valve 2? */
#define SHIFT_RELAY_BIG 8
#define SHIFT_LED_1     9
#define SHIFT_LED_2     10
#define SHIFT_LED_3     11
#define SHIFT_LED_4     12

/* Define the shift register */
Shift shift(SHIFT_CLOCK, SHIFT_LATCH, SHIFT_DATA, 2);

#ifdef OLD
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

#define NUM_POOFERS 2
State state(NUM_POOFERS);

Poofer poofers[NUM_POOFERS] = {
  Poofer(0, &state, &igniter_1_switch, &igniter_1_relay, &igniter_led,
         &valve_switch, &valve_relay, &valve_led, true),
};

#endif

Sensor valve_1_switch(VALVE_1_SWITCH, true, false, NULL);
Sensor igniter_1_switch(IGNITER_1_SWITCH, true, false, NULL);
Sensor valve_2_switch(VALVE_2_SWITCH, true, false, NULL);
Sensor igniter_2_switch(IGNITER_2_SWITCH, true, false, NULL);

#define NUM_PINS 14 // Digital pins only
// #define NUM_PINS 21 // Digital + analog
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,               // D0: RX - to Xbee
  NULL,               // D1: TX - to Xbee
  NULL,       // D2: 
  NULL,     // D3: 
  NULL,               // D4: 
  NULL,               // D5: 
  &valve_2_switch,    // D6: 
  &igniter_2_switch,  // D7: 
  &valve_1_switch,    // D8: 
  &igniter_1_switch,  // D9: 
  NULL,               // D10:  Shift register
  NULL,               // D11:  Shift register
  NULL,               // D12: Shift register
  NULL,               // D13: 

  /* Analog Pins */
#if 0
  NULL,               // A0: Empty
  NULL,               // A1: Empty
  NULL,               // A2: Empty
  NULL,               // A3: Empty
  NULL,               // A4: Empty
  NULL,               // A5: Empty
  NULL,               // A6: Empty
  NULL,               // A7: Empty
#endif
};

LiquidCrystal lcd(0);

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("Gigsville");

}

void loop() {

#if 1
  #if 0
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
  #endif
  shift.SetBit(SHIFT_RELAY_1, !valve_1_switch.read());
  shift.SetBit(SHIFT_RELAY_2, !igniter_1_switch.read());
  shift.SetBit(SHIFT_RELAY_3, !valve_2_switch.read());
  shift.SetBit(SHIFT_RELAY_4, !igniter_2_switch.read());
  shift.Write();
#else
  static unsigned int shiftvalue = 0;
//  shiftWrite(1 << shiftvalue % 13);

  shift.SetBit(shiftvalue, 0);
  shiftvalue = (shiftvalue + 1) % 13;
  shift.SetBit(shiftvalue, 1);
  shift.Write();
  delay(250);
#endif
}
