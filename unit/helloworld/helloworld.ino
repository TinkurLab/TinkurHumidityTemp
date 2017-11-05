void setup() {
  pinMode(0, OUTPUT);
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  digitalWrite(0, HIGH);
  Serial.println("high");
  delay(500);
  digitalWrite(0, LOW);
  Serial.println("low");
  delay(500);
}
