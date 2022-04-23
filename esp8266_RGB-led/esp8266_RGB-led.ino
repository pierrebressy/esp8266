/*********
  C.BRESSY 23.04.2022
  Tests control RGB Led Common cathode

*********/
int RGBLED_RED   = 5;
int RGBLED_GREEN = 4;
int RGBLED_BLUE  = 0;

void setup() {
  pinMode(RGBLED_RED, OUTPUT);
  pinMode(RGBLED_GREEN, OUTPUT);
  pinMode(RGBLED_BLUE, OUTPUT);
}

void loop() {
  analogWrite(RGBLED_RED, 0);
  analogWrite(RGBLED_GREEN, 255);
  analogWrite(RGBLED_BLUE, 255);

}
