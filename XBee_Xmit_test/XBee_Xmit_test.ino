#define RECV_LED 12
#define DEBUG_LED 13

void setup() {
  pinMode(RECV_LED, OUTPUT);
  pinMode(DEBUG_LED, OUTPUT);
  Serial.begin(9600);
}

int ontime = 0;
int debug_time = 0;

int count = 0;
void loop() {
  Serial.print("Test ");
  Serial.print(count);
  Serial.print("\n");

  delay(1000);
  count++;
}
