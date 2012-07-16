#ifndef POOFER_CONTROL_H
#define POOFER_CONTROL_H

#include "State.h"

class PooferControl {
  public:
    PooferControl(int id,
		  int ignLedPin,
		  int ignTogglePin,
		  int poofLedPin,
		  int poofActiveLedPin,
		  int poofButtonPin,
		  State &state);

    void updateState();
 
  private:
    int _id;

    int _ignLedPin;
    int _ignTogglePin;

    int _poofLedPin;
    int _poofActiveLedPin;
    int _poofButtonPin;

    State &_state;
};

#endif
