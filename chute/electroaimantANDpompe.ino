int button = 3;
int pump = 4;
int magnet = 13;

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(pump, OUTPUT);
  pinMode(magnet, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(button) == LOW) {
    digitalWrite(magnet, HIGH);
    delay(3000);
    digitalWrite(magnet, LOW);
  };
}

void serialEvent() {
  switch (Serial.read()) {
    case '1':
      digitalWrite(pump, HIGH);
      delay(3000);
      digitalWrite(pump, LOW);
      break;
    case '0':
      break;
  };
}
