
#include "Arduino.h"
#include "State.h"

State::State(int numPoofers) {
  _numPoofers = numPoofers > MAX_POOFERS ? MAX_POOFERS : numPoofers;
  for (int i = 0; i < numPoofers; i++) {
    _poofers[i] = 0;
    _newPoofers[i] = 0;
  }
}

void State::set_ign(int index, boolean on) {
  if (index < _numPoofers) {
    if (on) {
      _poofers[index] |= IGN_STATE;
    } else {
      _poofers[index] &= ~IGN_STATE;
    }
   }
}

void State::set_poof(int index, boolean on) {
  if (index < _numPoofers) {
    if (on) {
      _poofers[index] |= POOF_STATE;
    } else {
      _poofers[index] &= ~POOF_STATE;
    }
   }
}

boolean State::get_ign(int index) {
  if (index < _numPoofers) {
    return _poofers[index] & IGN_STATE;
  }
  return false;
}

boolean State::get_poof(int index) {
  if (index < _numPoofers) {
    return _poofers[index] & POOF_STATE;
  }
  return false;
}

void State::transmit() {
  char baseChar = BASE_CHAR;
  for(int x = 0; x < _numPoofers; x++) {
    if (_poofers[x] & IGN_STATE) {
      Serial.write(baseChar + x);
    }
  }

  baseChar = BASE_CHAR + _numPoofers;
  for (int y = 0; y < _numPoofers; y++) {
    if (_poofers[y] & POOF_STATE) {
      Serial.write(baseChar + y);
    }
  }

  Serial.write(DELIMITER);
}

void State::receive() {
  while (Serial.available()) {
    char value = Serial.read();
    if ((value >= BASE_CHAR) && (value < BASE_CHAR + _numPoofers)) {
      int index = value - BASE_CHAR;
      _newPoofers[index] |= IGN_STATE;
    } else if ((value >= BASE_CHAR + _numPoofers) && (value < BASE_CHAR + (2 * _numPoofers))) {
      int index = value - (BASE_CHAR + _numPoofers);
      _newPoofers[index] |= POOF_STATE;
    } else if (value == DELIMITER) {
      for (int i = 0; i < _numPoofers; i++ ) {
        _poofers[i] = _newPoofers[i];
        _newPoofers[i] = 0;
      }
    } else {
      for (int i = 0; i < _numPoofers; i++) {
        _newPoofers[i] = 0;
      }
    }
  }  
}