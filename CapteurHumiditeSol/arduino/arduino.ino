int sensorPin = A0;
int value;

void setup()
{

  pinMode(sensorPin, INPUT);

  Serial.begin(9600);
  
}

void loop()
{
  delay(1000);
  value = analogRead(sensorPin);
  Serial.println(value);
}

