#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "LiquidCrystal.h"

#define MAX_POOFERS 10

#define BASE_CHAR 'A'
#define START_CHAR 'Y'
#define DELIMITER 'X'
#define ACK_CHAR 'Z'

#define IGN_STATE  0x1
#define POOF_STATE 0x2

class State
{
  public:
  State(int numPoofers, LiquidCrystal *lcd);
    void set_ign(int index, boolean on);
    void set_poof(int index, boolean on);
    boolean get_ign(int index);
    boolean get_poof(int index);

    void transmit();
    void receive();

  private:
    int _numPoofers;
    byte _poofers[MAX_POOFERS];
    byte _newPoofers[MAX_POOFERS];
    LiquidCrystal *_lcd;
    uint8_t _sequence;
    uint8_t _last_acked;
};

#endif 
