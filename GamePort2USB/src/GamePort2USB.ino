
/*
  Game port -> USB, Arduino Leonardo sketch
  Game port pinout reference : 
  http://pinouts.ru/Inputs/GameportPC_pinout.shtml
  
  Les boutons relient directement le +5V a leur pin (2/7), il
  faut ajouter une resistance de 
  Les axes sont des resistances variables de 100k dont une
  extremite est au +5V et le point milieu est relie a la pin (3/6).
  L'autre extremite n'est pas connectee.

  Il faut donc dans tous les cas ajouter des resistances de
  pull-down (ici toutes de 100k) entre les pins et la Gnd pour
  fermer proprement les circuits.
  Pour les axes, la tension mesuree est 5V lorsque la resistance
  variable est a 0 et 2,5V lorsqu'elle est a 100k.
  On suppose ici le point milieu du joystick correctement calibre
  a 50k.

  Power mapping :
  Joy pin 1 -> +5V
  Joy pins 4 & 5 -> Gnd

  -----

  Pour la reconnaissance du joystick USB, il faut utiliser la specification
  USB HID. La lib Arduino ne fournit que la prise en charge des souris et
  clavier via USB HID. On utilise ici un code ajoutant la prise en charge
  des joysticks :
  http://www.imaginaryindustries.com/blog/?p=80
  (a mettre dans /usr/share/arduino/hardware/arduino/cores/arduino/)

*/

#define ANALOW 512
#define ANALIGH 1023
#define DELTANA (ANALOW+(ANALIGH-ANALOW)/2)
#define MOVETHRESH 30

// Joystick pin 2
#define BUTTON1 2
// Joystick pin 7
#define BUTTON2 3
// Joystick pin 3
#define AXEX1 A0
// Joystick pin 6
#define AXEY1 A1

JoyState_t J;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  pinMode(AXEX1,INPUT);
  pinMode(AXEY1,INPUT);

  J.xAxis = 0;
  J.yAxis = 0;
  J.zAxis = 0;
  J.xRotAxis = 0;
  J.yRotAxis = 0;
  J.zRotAxis = 0;
  J.throttle = 0;
  J.rudder = 0;
  J.hatSw1 = 0;
  J.hatSw2 = 0;
  J.buttons = 0;
}

void loop() {
  /*  Serial.println(digitalRead(BUTTON1));
      Serial.println(digitalRead(BUTTON2));
      Serial.println(analogRead(AXEX1));
      Serial.println(analogRead(AXEY1));
  */
  J.xAxis = map(analogRead(AXEX1),ANALOW,ANALIGH,0,255);
  J.yAxis = map(analogRead(AXEY1),ANALOW,ANALIGH,0,255);
  J.buttons = ((digitalRead(BUTTON1)==HIGH)?1:0) | (((digitalRead(BUTTON2)==HIGH)?1:0)<<1);
  Joystick.setState(&J);
  delay(100);
}
