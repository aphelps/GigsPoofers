#include "Wire.h"
#include "LiquidCrystal.h"
LiquidCrystal lcd(0);

//Pin connected to ST_CP of 74HC595
int latchPin = 13;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

#define NUM_SWITCHES 9
int switchPin[NUM_SWITCHES] = {2,3,4,5,6,7,8,9,10};


void setup() {
  Serial.begin(9600);

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switchPin[i], INPUT_PULLUP);
  }
  
  digitalWrite(latchPin, LOW);
  
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  // Print a message to the LCD.
  lcd.print("Hello Gigsville!");
}

int delay_ms = 250;
int cycle = 0;
int mode = 1;

#define NUMLEDS 8

void sendLatch(void) {
  static int latch = LOW;
  
  latch = !latch;
  digitalWrite(latchPin, latch);
}

void shiftWrite(uint32_t value) {
  byte low;  
  low = value & 0x00FF;
  
  shiftOut(dataPin, clockPin, MSBFIRST, low);  

  sendLatch();
}

void loop() {
  byte low = 0;
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000 % 100);
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
      } else {
        low = 0xFF;
      }  
      break;
    }

    case 1: {
      // Cycle through the outputs
      int led = 1 << (cycle % NUMLEDS);
      low = led & 0x00FF;
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

  lcd.print("-");
  lcd.print(low);
  lcd.print(" ");
  shiftOut(dataPin, clockPin, MSBFIRST, low);  

  sendLatch();
  sendLatch();
    
//  Serial.print(low);
  
  while (Serial.available()) {
    char value = Serial.read();
    
    Serial.print(value);
    if (value == 'X') Serial.println();
    lcd.print(value);
  }
  
  delay(delay_ms);
  cycle++;
}

