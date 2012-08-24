#include <XBee.h>
\
// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 'A', 'B', 'C', 'D' };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0, 0x0);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();


int statusLed = 12;
int debugLed = 11;

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(debugLed, OUTPUT);

  xbee.begin(9600);
}

void flashLed(int pin, int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}


void loop() {   

  xbee.send(zbTx);

  // flash TX indicator
  digitalWrite(statusLed, HIGH);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!
    flashLed(statusLed, 4, 250);
    digitalWrite(debugLed, HIGH);
  }

//  digitalWrite(statusLed, LOW);

  delay(1000);
}

