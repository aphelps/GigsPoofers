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

//#define DEBUG
//#define DEBUG_VERBOSE 2

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
#define SHIFT_RELAY_1   0
#define SHIFT_RELAY_2   1
#define SHIFT_RELAY_3   2
#define SHIFT_RELAY_4   3
#define SHIFT_RELAY_5   4
#define SHIFT_RELAY_6   5
#define SHIFT_RELAY_7   6
#define SHIFT_RELAY_8   7
#define SHIFT_RELAY_BIG 8
#define SHIFT_LED_1     9
#define SHIFT_LED_2     10
#define SHIFT_LED_3     11
#define SHIFT_LED_4     12

#define SHIFT_VALVE_1   SHIFT_RELAY_1
#define SHIFT_IGNITER_1 SHIFT_RELAY_2
#define SHIFT_VALVE_2   SHIFT_RELAY_3
#define SHIFT_IGNITER_2 SHIFT_RELAY_4
#define SHIFT_SPOTLIGHT SHIFT_RELAY_BIG

Shift shift(SHIFT_CLOCK, SHIFT_LATCH, SHIFT_DATA, 2);

Output valve_1_relay(SHIFT_VALVE_1, LOW, &shift);
Output igniter_1_relay(SHIFT_IGNITER_1, LOW, &shift);
Output valve_2_relay(SHIFT_VALVE_2, LOW, &shift);
Output igniter_2_relay(SHIFT_IGNITER_2, LOW, &shift);
Output spot_relay(SHIFT_SPOTLIGHT, LOW, &shift);

#define NUM_OUTPUTS 5
Pin *outputs[NUM_OUTPUTS] = {
  &valve_1_relay,
  &igniter_1_relay,
  &valve_2_relay,
  &igniter_2_relay,
  &spot_relay
};


Sensor valve_1_switch(VALVE_1_SWITCH, true, false, false, NULL, NULL);
Sensor igniter_1_switch(IGNITER_1_SWITCH, true, false, NULL);
Sensor valve_2_switch(VALVE_2_SWITCH, true, false, false, NULL, NULL);
Sensor igniter_2_switch(IGNITER_2_SWITCH, true, false, NULL);

LiquidCrystal lcd(0);


#define NUM_POOFERS 2
State state(NUM_POOFERS, &lcd);
Poofer poofers[NUM_POOFERS] = {
  Poofer(0, &state,
         &igniter_1_switch, &igniter_1_relay,
         &valve_1_switch, &valve_1_relay, false),
  Poofer(1, &state,
         &igniter_2_switch, &igniter_2_relay,
         &valve_2_switch, &valve_2_relay, false)
};


#define NUM_PINS 14 // Digital pins only
// #define NUM_PINS 21 // Digital + analog
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,               // D0: RX - to Xbee
  NULL,               // D1: TX - to Xbee
  NULL,               // D2: 
  NULL,               // D3: 
  NULL,               // D4: 
  NULL,               // D5: 
  &valve_2_switch,    // D6: 
  &igniter_2_switch,  // D7: 
  &valve_1_switch,    // D8: 
  &igniter_1_switch,  // D9: 
  NULL,               // D10: Shift register
  NULL,               // D11: Shift register
  NULL,               // D12: Shift register
  NULL,               // D13:

  /* Analog Pins */
#if 0
  NULL,               // A0: Empty - To screw
  NULL,               // A1: Empty
  NULL,               // A2: Empty
  NULL,               // A3: Empty
  NULL,               // A4: I2C - Serial LCD
  NULL,               // A5: I2C - Serial LCD
  NULL,               // A6: Empty
  NULL,               // A7: Empty
#endif
};

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.print("Gigsville");
}

void loop() {
#if 1
  /* Check sensores and perform actions */
  checkSensors(pinArray, NUM_PINS, false);

  /* Receive commands via XBee */
  state.receive();

  /* Based on the inputs determined what to do with the poofers */
  for (int i = 0; i < NUM_POOFERS; i++) {
    poofers[i].processState();
  }

  /* Trigger the outputs and update the shit register */
  triggerOutputs(outputs, NUM_OUTPUTS);
  shift.Write();

  /* Set status display */
  lcd.setCursor(0, 1);
  for (int i = 0; i < NUM_POOFERS; i++) {
//    lcd.print(i);
    lcd.print("I");
    lcd.print(poofers[i].getIgn());
    lcd.print(state.get_ign(i));
    lcd.print("V");
    lcd.print(poofers[i].getSol());
    lcd.print(state.get_poof(i));
    lcd.print("");
  }

  DEBUG_COMMAND(delay(100));
#else
  /* Check sensores and perform actions */
  checkSensors(pinArray, NUM_PINS, false);

  /* Receive updates from the brain */
  // XXX

  /* Send status to the brain */
  // XXX

  /* Trigger the outputs and update the shit register */
  triggerOutputs(outputs, NUM_OUTPUTS);
  shift.Write();

  /* Set status display */
  lcd.setCursor(0, 1);
  lcd.print("S:");
  for (int i = 0; i < NUM_PINS; i++) {
    Pin *pin = pinArray[i];
    if (pin && (pin->type == PIN_TYPE_SENSOR)) {
      lcd.print(((Sensor *)pin)->read());
    }
  }

  DEBUG_COMMAND(delay(100));
#endif
}

#if 0
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
#endif
