//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

#include "Wire.h"
#include "LiquidCrystal.h"
LiquidCrystal lcd(0);


//Pin connected to ST_CP of 74HC595
int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int debugPin = 13;
int debugOn = 0;

#define NUM_SWITCHES 4
int switchPin[NUM_SWITCHES] = {9,8,7,6};

void setup() {
  Serial.begin(9600);

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(debugPin, OUTPUT);
  
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switchPin[i], INPUT_PULLUP);
  }
  
  digitalWrite(latchPin, LOW);
  
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  // Print a message to the LCD.
  lcd.print("Hello Gigsville!");
}

int delay_ms = 500;
int cycle = 0;
int mode = 1;

#define NUMLEDS 13

void sendLatch(void) {
  static int latch = LOW;
  
  latch = !latch;
  digitalWrite(latchPin, latch);
}

void shiftWrite(uint32_t value) {
  byte low, high;  
  low = value & 0x00FF;
  high = (value >> 8);
  
  shiftOut(dataPin, clockPin, MSBFIRST, high);        
  shiftOut(dataPin, clockPin, MSBFIRST, low);  

  sendLatch();
  sendLatch();
}

void loop() {
  byte low = 0;
  byte high = 0;
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  lcd.print("-");
  for (int i = 0; i < NUM_SWITCHES; i++) {
    lcd.print(digitalRead(switchPin[i]));
  }

//  digitalWrite(latchPin, LOW);
//  sendLatch();

  switch (mode) {
    case 0: {
      if (cycle % 2) {
        low = 0;
        high = 0xFF;
      } else {
        low = 0xFF;
        high = 0;
      }  
      break;
    }


    case 1: {
      // Cycle through the outputs
      int led = 1 << (cycle % NUMLEDS);
      low = led & 0x00FF;
      high = (led >> 8);
      break;
    }
    
    case 2: {
      for (int val= 0; val < (1 << NUMLEDS); val++) {
        shiftWrite(val);
        delay(100);
      }  
      return;
    }
  }

  shiftOut(dataPin, clockPin, MSBFIRST, high);        
  shiftOut(dataPin, clockPin, MSBFIRST, low);  

  sendLatch();
  sendLatch();

    
//  Serial.print(low);
//  Serial.println(high);
  
  if (debugOn) {
    digitalWrite(debugPin, HIGH); 
    debugOn = 0;
  } else {
    digitalWrite(debugPin, LOW);      
    debugOn = 1;
  }

  while (Serial.available()) {
    char value = Serial.read();
    
    Serial.print(value);
    if (value == 'X') Serial.println();
    lcd.print(value);
    
    //digitalWrite(RECV_LED, HIGH);
  }

  
  delay(delay_ms);
  cycle++;
}

