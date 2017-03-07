int button = 3;
int electromagnet = 13;

void setup() {
  pinMode(electromagnet, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  while(digitalRead(button) == HIGH) {};
  digitalWrite(electromagnet, HIGH);
  delay(3000);
  digitalWrite(electromagnet, LOW);
}
