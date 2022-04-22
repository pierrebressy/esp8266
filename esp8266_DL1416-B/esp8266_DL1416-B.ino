/*********
  C.BRESSY 22.04.2022
  Tests afficheur DL1416 B
  # Doc DL1416-B :
  # PIN   FUNCTION
  #  1    DATA D5         # 11    A1
  #  2    DATA D4         # 12    Unused
  #  3    DATA D0         # 13    Unused
  #  4    DATA D1         # 14    Unused
  #  5    DATA D2         # 15    Unused
  #  6    DATA D3         # 16    Unused
  #  7    Chip enable     # 17    Unused
  #  8    Write           # 18    V+
  #  9    Cursor input    # 19    V-
  # 10    Digit select    # 20    DATA D6
  Useful informations from documentation :
  - TW (Time to Write) says at 25 degrees it's at least 0,25 microseconds
  - D0 to D6 you should wait 50 nanoseconds before OFF write
*********/
#include <ESP8266WiFi.h>

#include <Ticker.h>  //Ticker Library


#define BUILT_IN_BLINK_ENABLED 1
#define NUM_DIGIT 4



// GPIOs pins definition
// int GPIO_CHIP_ENABLE = 12;  // Removed cause GPIO_09 and GPIO_10 can ONLY receive data (no output)
int GPIO_WRITE = 14;
int GPIO_DIGIT_SELECT_A0 = 13;
int GPIO_DIGIT_SELECT_A1 = 12;

int GPIO_D0 = 0;
int GPIO_D1 = 1;
int GPIO_D2 = 2;
int GPIO_D3 = 3;
int GPIO_D4 = 4;
int GPIO_D5 = 5;
int GPIO_D6 = 15;  // Did not found gpio_6 ?

int SLEEPING_TIME = 1; // #seconds


Ticker myClock;


unsigned int h = 0;
unsigned int m = 0;
unsigned int s = 0;
unsigned int x = 0;
char ss[NUM_DIGIT + 1] = "0000";

void chip_enable() {
  //digitalWrite(GPIO_CHIP_ENABLE, HIGH); // sets the pin on
}

void chip_disable() {
  //digitalWrite(GPIO_CHIP_ENABLE, LOW); // sets the pin off
}

void write_enable() {
  digitalWrite(GPIO_WRITE, HIGH); // Enable
  delayMicroseconds(50);          // pauses for 50 microseconds
}

void write_disable() {
  digitalWrite(GPIO_WRITE, LOW);  // Disable
  delayMicroseconds(50);          // pauses for 50 microseconds
}

void digit_select(int digit) {
  if (digit > 3 || digit < 0) {
    return; // digit should be in 0-3 range.
  }

  // Désactivation des pins
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable

  if (digit == 0) {
    digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  }

  if (digit == 1) {
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
  }

  if (digit == 2) {
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
  }

  if (digit == 3) {
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
  }
}

void digit_unselect() {
  //  Désactivation des deux digit select
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable
}

void set_data(int a, int b, int c, int d, int e, int f, int g) {
  digitalWrite(GPIO_D0, a);
  digitalWrite(GPIO_D1, b);
  digitalWrite(GPIO_D2, c);
  digitalWrite(GPIO_D3, d);
  digitalWrite(GPIO_D4, e);
  digitalWrite(GPIO_D5, f);
  digitalWrite(GPIO_D6, g);
}


void updateClockAndDisplay()
{
  sprintf(s, "%02d%02d", m,s);
  for (int digit = 0; digit < NUM_DIGIT; digit++) {
    chip_disable();
    digit_select(digit);
    x = s[NUM_DIGIT - 1 - digit];
    //Serial.println(x);
    set_data(x & 0x01 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x40 ? 1 : 0);
    write_disable();
    delayMicroseconds(50);      // pauses for 50 microseconds
    write_enable();
    digit_unselect();
    chip_enable();

    delayMicroseconds(50);      // pauses for 50 microseconds
  }

  s++;
  if(s>59) {
    s=0;
    m++;
  }
  if(m>59) {
    m=0;
    h++;
  }
  return;
}



void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  // initialize GPIOs as outputs.
  // pinMode(GPIO_CHIP_ENABLE, OUTPUT);
  pinMode(GPIO_WRITE, OUTPUT);
  pinMode(GPIO_DIGIT_SELECT_A0, OUTPUT);
  pinMode(GPIO_DIGIT_SELECT_A1, OUTPUT);

  pinMode(GPIO_D0, OUTPUT);
  pinMode(GPIO_D1, OUTPUT);
  pinMode(GPIO_D2, OUTPUT);
  pinMode(GPIO_D3, OUTPUT);
  pinMode(GPIO_D4, OUTPUT);
  pinMode(GPIO_D5, OUTPUT);
  pinMode(GPIO_D6, OUTPUT);

  chip_enable();
  write_disable();

  // ESP.wdtDisable();

  // Setup Wifi
  digitalWrite(LED_BUILTIN, HIGH);

  //Serial.begin(115200);
  //Serial.println();

  WiFi.begin("RANTANPLAN", "F5QiRNX1rCf9iqNaYg");

  //Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println();

  //Serial.print("Connected, IP address: ");
  //Serial.println(WiFi.localIP());

  myClock.attach(1.0, updateClockAndDisplay); 

}

// Switch name with next function for easy gpio debugging
void loop_debug() {
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW);

  delayMicroseconds(250000);
  digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH);
  delayMicroseconds(250000);
}








// The loop function runs over and over again forever
void loop() {
  ESP.wdtFeed();
  delayMicroseconds(100000); // 100ms

#if 0

  // Lowers builtin led intensity
#if BUILT_IN_BLINK_ENABLED
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(12345);
  digitalWrite(LED_BUILTIN, LOW);
#endif


#if 1
  sprintf(ss, "%02d%02d", m,s);
  //Serial.println(s);
  for (int digit = 0; digit < NUM_DIGIT; digit++) {
    chip_disable();
    digit_select(digit);
    x = ss[NUM_DIGIT - 1 - digit];
    //Serial.println(x);
    set_data(x & 0x01 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x40 ? 1 : 0);
    write_disable();
    delayMicroseconds(50);      // pauses for 50 microseconds
    write_enable();
    digit_unselect();
    chip_enable();

    delayMicroseconds(50);      // pauses for 50 microseconds
  }

  s++;
  if(s>59) {
    s=0;
    m++;
  }
  if(m>59) {
    m=0;
    h++;
  }
  delayMicroseconds(125000);      // pause for 200 ms

#else
  // Digit 0 (Right)
  chip_disable();
  digit_select(0);
  x = (count & 0x000F) >> 0;
  set_data(x & 0x40 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x01 ? 1 : 0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 1
  chip_disable();
  digit_select(1);
  x = (count & 0x00F0) >> 4;
  set_data(x & 0x40 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x01 ? 1 : 0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 2
  chip_disable();
  digit_select(2);
  x = (count & 0x0F00) >> 8;
  set_data(x & 0x40 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x01 ? 1 : 0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 3 (Left)
  chip_disable();
  digit_select(3);
  x = (count & 0xF000) >> 12;
  set_data(x & 0x40 ? 1 : 0, x & 0x20 ? 1 : 0, x & 0x10 ? 1 : 0, x & 0x08 ? 1 : 0, x & 0x04 ? 1 : 0, x & 0x02 ? 1 : 0, x & 0x01 ? 1 : 0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds
#endif
  
  
#endif

}
