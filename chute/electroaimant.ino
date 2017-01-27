int button = 3;
int electroAimant = 13;

void setup() {
  pinMode(electroAimant, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  while(digitalRead(button) == HIGH) {};
  digitalWrite(electroAimant, HIGH);
  delay(3000);
  digitalWrite(electroAimant, LOW);
}
