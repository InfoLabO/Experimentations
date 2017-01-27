int button = 3;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  while(digitalRead(3) == HIGH) {};
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
}
