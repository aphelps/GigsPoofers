#include <XBee.h>
\
// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 'A', 'B', 'C', 'D' };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x7d33a2, 0x408d1c51);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();


int statusLed = 13;
int debugLed = 12;

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(debugLed, OUTPUT);

  xbee.begin(9600);

  flashLed(statusLed, 4, 500);
  flashLed(debugLed, 4, 500);
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
  flashLed(statusLed, 4, 250);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  while (xbee.readPacket(500)) {
    int responseCode = xbee.getResponse().getApiId();
    Serial.print("\nDATA:");
    Serial.print(responseCode);

    switch (responseCode) {
        case ZB_TX_STATUS_RESPONSE:
          xbee.getResponse().getZBTxStatusResponse(txStatus);

          // get the delivery status, the fifth byte
          if (txStatus.getDeliveryStatus() == SUCCESS) {
            // success.  time to celebrate
            Serial.print("TX Success");
          } else {
            // the remote XBee did not receive our packet. is it powered on?
            Serial.print("TX Fail");
          }

          break;

        case ZB_RX_RESPONSE:
          Serial.print("RESPONSE!");
          flashLed(debugLed, 4, 1000);
          // now fill our zb rx class
          xbee.getResponse().getZBRxResponse(rx);
          Serial.print("-");
          Serial.print(rx.getDataLength());
          Serial.print("-");
          uint8_t *data = rx.getData();
          for (int i = 0; i < rx.getDataLength(); i++) 
          {
            Serial.print(data[i]);
            if (data[i] == 'A') {
              digitalWrite(debugLed, HIGH);
            }
          }
          break;
      
    }
    Serial.println();
    flashLed(statusLed, 4, 100);
  }

//  digitalWrite(statusLed, LOW);

  delay(1000);
}


