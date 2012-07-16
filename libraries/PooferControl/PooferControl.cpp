#include "Arduino.h"
#include "PooferControl.h"

PooferControl::PooferControl(int id,
		             int ignLedPin,
		             int ignTogglePin,
		             int poofLedPin,
		             int poofActiveLedPin,
		             int poofButtonPin,
		             State &state): _state(state) {
  _id = id;
  
  _ignLedPin = ignLedPin;
  pinMode(_ignLedPin, OUTPUT);
  
  _ignTogglePin = ignTogglePin;
  pinMode(_ignTogglePin, INPUT_PULLUP);

  _poofLedPin = poofLedPin;
  pinMode(_poofLedPin, OUTPUT);
  
  _poofActiveLedPin = poofActiveLedPin;
  pinMode(_poofActiveLedPin, OUTPUT);

  _poofButtonPin = poofButtonPin;
  pinMode(_poofButtonPin, INPUT_PULLUP);
}

void PooferControl::updateState() {
  int ignState = digitalRead(_ignTogglePin);
  
  //LOW = depressed
  _state.set_ign(_id, ignState == LOW);
  digitalWrite(_ignLedPin, ignState == LOW ? HIGH : LOW);

  // If the igniter is on, prevent poofing
  if (ignState == LOW) {
    _state.set_poof(_id, false);
    digitalWrite(_poofLedPin, false);
    digitalWrite(_poofActiveLedPin, false);
    return;
  } else {
    digitalWrite(_poofActiveLedPin, true);
  }

  int poofState = digitalRead(_poofButtonPin);
  _state.set_poof(_id, poofState == LOW);
  digitalWrite(_poofLedPin, poofState == LOW ? HIGH : LOW);
}
