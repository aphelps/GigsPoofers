/******************************************************************************
 * The final control box for Gigsville Poofers in 2012
 *
 * Pan ID: 4747 "GG"
 * XBee Pro: Coordinator API
 *   SL: 40982416
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

LiquidCrystal lcd(0);

/* Pins */
#define VALVE_1_SWITCH    2
#define IGNITER_1_SWITCH  5
#define VALVE_2_SWITCH    4
#define IGNITER_2_SWITCH  3
#define SPOTLIGHT_SWITCH  10
//#define EXTRA_SWITCH      128// Not connected?
#define MENU_UP_SWITCH    8
#define MENU_DOWN_SWITCH  7
#define MENU_ENTER_SWITCH 6
#define SHIFT_DATA        11
#define SHIFT_CLOCK       12
#define SHIFT_LATCH       13

#define REED_SWITCH       9

/* Shift register outputs */
#define SHIFT_BUTTON_1    1
#define SHIFT_VALVE_1     5
#define SHIFT_IGNITER_1   4
#define SHIFT_BUTTON_2    2
#define SHIFT_VALVE_2     3
#define SHIFT_IGNITER_2   6
#define SHIFT_SPOTLIGHT   7
#define SHIFT_EXTRA       0

Shift shift(SHIFT_CLOCK, SHIFT_LATCH, SHIFT_DATA, 1);

Sensor valve_1_switch(VALVE_1_SWITCH,      true, false, NULL);
Sensor igniter_1_switch(IGNITER_1_SWITCH,  true, false, NULL);
Sensor valve_2_switch(VALVE_2_SWITCH,      true, false, NULL);
Sensor igniter_2_switch(IGNITER_2_SWITCH,  true, false, NULL);
Sensor spotlight_switch(SPOTLIGHT_SWITCH,  true, false, NULL);
//Sensor extra_switch(EXTRA_SWITCH,          true, false, NULL);
Sensor menu_up_switch(MENU_UP_SWITCH,      true, false, NULL);
Sensor menu_down_switch(MENU_DOWN_SWITCH,  true, false, NULL);
Sensor menu_enter_switch(MENU_ENTER_SWITCH, true, false, NULL);

Output button_1_led(SHIFT_BUTTON_1, HIGH, &shift);
Output valve_1_led(SHIFT_VALVE_1, LOW, &shift, &valve_1_switch);
Output igniter_1_led(SHIFT_IGNITER_1, LOW, &shift, &igniter_1_switch);
Output button_2_led(SHIFT_BUTTON_2, HIGH, &shift);
Output valve_2_led(SHIFT_VALVE_2, LOW, &shift, &valve_2_switch);
Output igniter_2_led(SHIFT_IGNITER_2, LOW, &shift, &igniter_2_switch);
Output spotlight_led(SHIFT_SPOTLIGHT, LOW, &shift, &spotlight_switch);
Output extra_led(SHIFT_EXTRA, LOW, &shift);

#define NUM_OUTPUTS 8
Pin *outputs[NUM_OUTPUTS] = {
  &button_1_led,
  &valve_1_led,
  &igniter_1_led,
  &button_2_led,
  &valve_2_led,
  &igniter_2_led,
  &spotlight_led,
  &extra_led
};


#define NUM_PINS 14 // Digital pins only
// #define NUM_PINS 21 // Digital + analog
Pin *pinArray[NUM_PINS] = {
  /* Digital Pins */
  NULL,               // D0: RX - to Xbee
  NULL,               // D1: TX - to Xbee
  &valve_1_switch,    // D2: 
  &igniter_1_switch,  // D3: 
  &valve_2_switch,    // D4: 
  &igniter_2_switch,  // D5:
  &spotlight_switch,  // D6: 
  NULL, //&extra_switch,      // D7: 
  &menu_up_switch,    // D8: 
  &menu_down_switch,  // D9:
  &menu_enter_switch, // D10:
  NULL,               // D11: Shift register
  NULL,               // D12: Shift register
  NULL,               // D13: Shift register

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
#else
  /* Check sensores and perform actions */
  checkSensors(pinArray, NUM_PINS, false);

  lcd.setCursor(0, 1);
  lcd.print("S:");
  if (valve_1_switch.curr_state== HIGH) {
    valve_1_led.setValue(HIGH);
    lcd.print("X");
  } else {
    valve_1_led.setValue(LOW);
    lcd.print("O");
  }
  
  if (igniter_1_switch.curr_state == HIGH) {
    igniter_1_led.setValue(HIGH);
    lcd.print("X");
  } else {
    igniter_1_led.setValue(LOW);
    lcd.print("O");
  }

  if (valve_2_switch.read() == HIGH) {
    valve_2_led.setValue(HIGH);
    lcd.print("X");
  } else {
    valve_2_led.setValue(LOW);
    lcd.print("O");
  }

  if (igniter_2_switch.read() == HIGH) {
    igniter_2_led.setValue(HIGH);
    lcd.print("X");
  } else {
    igniter_2_led.setValue(LOW);
    lcd.print("O");
  }

  triggerOutputs(outputs, NUM_OUTPUTS);
  shift.Write();

  DEBUG_COMMAND(XXXxsdelay(100));
#endif
}
