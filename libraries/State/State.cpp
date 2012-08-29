//#define DEBUG
//#define DEBUG_VERBOSE 0

#include "Arduino.h"
#include "LiquidCrystal.h"


#include "State.h"
#include "Debug.h"


State::State(int numPoofers, LiquidCrystal *lcd) {
  _numPoofers = numPoofers > MAX_POOFERS ? MAX_POOFERS : numPoofers;
  for (int i = 0; i < numPoofers; i++) {
    _poofers[i] = 0;
    _newPoofers[i] = 0;
  }
  _lcd = lcd;
  _sequence = 0;
  _last_acked = 0;
}

void State::set_ign(int index, boolean on) {
  if (index < _numPoofers) {
    if (on) {
      if (!(_poofers[index] & IGN_STATE)) {
        _poofers[index] |= IGN_STATE;
        _sequence++;
      }
    } else {
      if ((_poofers[index] & IGN_STATE)) {
        _poofers[index] &= ~IGN_STATE;
        _sequence++;
      }
    }
   }
}

void State::set_poof(int index, boolean on) {
  if (index < _numPoofers) {
    if (on) {
      if (!(_poofers[index] & POOF_STATE)) {
        _poofers[index] |= POOF_STATE;
        _sequence++;
      }
    } else {
      if ((_poofers[index] & POOF_STATE)) {
        _poofers[index] &= ~POOF_STATE;
        _sequence++;
      }
    }
   }
}

boolean State::get_ign(int index) {
  if (index < _numPoofers) {
    return (_poofers[index] & IGN_STATE ? true : false);
  }
  return false;
}

boolean State::get_poof(int index) {
  if (index < _numPoofers) {
    return (_poofers[index] & POOF_STATE ? true : false);
  }
  return false;
}

void State::transmit() {
  if (_sequence == _last_acked)
    return;

  Serial.write(START_CHAR);
  Serial.write(_sequence);

  char baseChar = BASE_CHAR;
  char xmitChar = 0;
  for(int x = 0; x < _numPoofers; x++) {
    if (_poofers[x] & IGN_STATE) {
      xmitChar = baseChar + x;
      Serial.write(xmitChar);
    }
  }

  baseChar = BASE_CHAR + _numPoofers;
  for (int y = 0; y < _numPoofers; y++) {
    if (_poofers[y] & POOF_STATE) {
      xmitChar = baseChar + y;
      Serial.write(xmitChar);
    }
  }

  Serial.write(DELIMITER);
  DEBUG_PRINT(2, "\n"); // Need to add a new line
}

void State::receive() {
  char value;
  uint8_t _sequence_rcvd;
  boolean ack = false;

#ifdef DEBUG
  _lcd->setCursor(0, 0);
  _lcd->print("rcv:");
#endif

  while (Serial.available()) { // Detect packet
    value = Serial.read();

    if (value == START_CHAR) {

      if (Serial.available()) { // Read serial number
        value = Serial.read();
        _sequence_rcvd = value;
      } else {
        return;
      }

      while (Serial.available()) { // Read state
      value = Serial.read();
        DEBUG_PRINT(2, value);
#ifdef DEBUG
        // _lcd->print(value);
#endif

        if ((value >= BASE_CHAR) && (value < BASE_CHAR + _numPoofers)) {
          int index = value - BASE_CHAR;
          _newPoofers[index] |= IGN_STATE;
        } else if ((value >= BASE_CHAR + _numPoofers) &&
                   (value < BASE_CHAR + (2 * _numPoofers))) {
          int index = value - (BASE_CHAR + _numPoofers);
          _newPoofers[index] |= POOF_STATE;
        } else if (value == ACK_CHAR) {
          // Update ack'd sequence number
          _last_acked = _sequence_rcvd;
          ack = true;
        } else if (value == DELIMITER) {
          DEBUG_PRINT(2, "(done)\n");
          if (!ack) {
            for (int i = 0; i < _numPoofers; i++ ) {
              _poofers[i] = _newPoofers[i];
              _newPoofers[i] = 0;
            }

            /* Entire packet has been received */
            Serial.write(START_CHAR);
            Serial.write(_sequence_rcvd);
            Serial.write(ACK_CHAR);
            Serial.write(DELIMITER);
          }
          
        } else {
          for (int i = 0; i < _numPoofers; i++) {
            _newPoofers[i] = 0;
          }
        }
      }
    } // START_CHAR
}

#ifdef DEBUG
  _lcd->print(_sequence);
  _lcd->print("/");
  _lcd->print(_last_acked);
  _lcd->print("                   ");
#endif  
}

