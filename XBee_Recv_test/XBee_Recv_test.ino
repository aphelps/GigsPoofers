#define RECV_LED 8
#define DEBUG_LED 12

void setup() {
  pinMode(RECV_LED, OUTPUT);
  pinMode(DEBUG_LED, OUTPUT);
  Serial.begin(9600);
}

int ontime = 0;
int debug_time = 0;

void loop() {
  char value;
  if (Serial.available()) {
    value = Serial.read();
    
    Serial.print(value);
    if (value == 'X') Serial.println();
    
    digitalWrite(RECV_LED, HIGH);
    ontime = millis();
  } else {
    if (millis() - ontime > 1000) {
      digitalWrite(RECV_LED, LOW);
    } 
  }

  int elapsed = millis() - debug_time;
  if (debug_time == 0) {
    digitalWrite(DEBUG_LED, HIGH);
    debug_time = millis();
  } else if (elapsed > 1000 && elapsed < 2000) {
    digitalWrite(DEBUG_LED, LOW);
  } else if (elapsed > 2000) {
    elapsed = 0;
  }
}
