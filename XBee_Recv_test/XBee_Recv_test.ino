#define RECV_LED 4

void setup() {
  pinMode(RECV_LED, OUTPUT);  
  Serial.begin(9600);
}

int ontime = 0;

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
}
