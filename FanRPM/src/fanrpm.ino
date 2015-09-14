/*
  Mesure de la vitesse d'un ventilateur de PC (3-4 pins)
  par BLG en 08/2015

  Les ventilateurs de PC fonctionnent en 12V et ont 3 ou 4 pins
  (4 pour les ventilateurs de processeurs) dans cet ordre :
   1. masse (noir)
   2. +12V (rouge ou jaune)
   3. tachymetre
   4. controle PWM
 
   La specification officielle est ici :
   http://www.formfactors.org/developer/specs/REV1_2_Public.pdf

   La pin 3 permet de mesurer la "vitesse" (frequence de rotation)
   du ventilateur. Elle est connectee a la masse 2 fois par tour
   (i.e. durant 2 quarts de tour non consecutifs) et non connectee
   durant les 2 quarts de tour restant.
   Il faut donc une resistance de pull-up (e.g. 100k) entre la pin 3
   et le +5V (de l'Arduino) pour mesurer le signal tachymetrique (sans
   oublier de relier la masse de l'Arduino a celle de l'alimentation
   du ventilateur).

   Ici la pin 3 du ventilateur doit etre connectee a l'interruption 0,
   i.e. pin 2 sur Uno, 3 sur Leonardo.

   Ici on considere le delai fixe entre deux loop() comme fiable car
   la premiere instruction du bloc memorise la valeur du compteur et
   la derniere le remet a 0. Les interruptions ayant lieu au cours de
   l'execution du bloc (ecritures sur le port serie) ne sont donc pas
   comptabilisees (a moins qu'elles ne soient delayees, auquel cas il
   faudrait idealement soit mesurer le temps plus precisement, soit
   detacher les interruptions momentanement ; mais, en pratique, il
   suffit de prendre un delai suffisamment grand pour qu'elles soient
   negligeables).
*/

#define DELAI 1000

volatile int compteur = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  attachInterrupt(0,inc_cpt,FALLING);
}

void loop() {
  delay(DELAI);
  float freq = compteur/(DELAI/1000.);
  Serial.print(freq*30.);
  Serial.print(" rpm (");
  Serial.print(freq);
  Serial.println(" Hz)");
  compteur = 0;
}

void inc_cpt() {
  ++compteur;
}
