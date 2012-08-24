
#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();

int statusLed = 13;
int debugLed = 12;

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

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(debugLed, OUTPUT);
  
  // start serial
  xbee.begin(9600);
  
  flashLed(statusLed, 10, 500);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket(1000);
    
    if (xbee.getResponse().isAvailable()) {
      // got something

      Serial.print("AVAILABLE");
      
      flashLed(statusLed, 4, 250);
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
            
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
            flashLed(statusLed, 10, 100);
        }

        uint8_t *data = rx.getData();
        for (int i = 0; i < rx.getDataLength(); i++) 
        {
          Serial.print(data[i]);
          if (data[i] == 'A') {
            digitalWrite(debugLed, HIGH);
          }
        }
        
        // set dataLed PWM to value of the first byte in the data
        flashLed(statusLed, rx.getDataLength(), 500);
      }

      Serial.println();
    } else {
      Serial.println("NO AVAILABLE");
    }

//    flashLed(debugLed, 4, 250);
    
}
