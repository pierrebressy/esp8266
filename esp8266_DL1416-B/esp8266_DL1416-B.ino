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
#include <dht.h>
#include <Ticker.h>  //Ticker Library

#define dht_apin A0 // For dht.h
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


unsigned int hh = 0;
unsigned int mm = 0;
unsigned int ss = 0;
unsigned int x = 0;
char s[NUM_DIGIT + 1] = "0000";


int * get_representation(char car) {

  //static int  r[7];

  // returns             a,b,c,d,e,f,g (D0 to D6)

  // 1st column of documentation's table
  if (car == ' ') {
    int ret[] = {0, 0, 0, 0, 0, 1, 0};
  }
  if (car == '<') {
    int ret[] = {0, 0, 0, 1, 0, 1, 0};
  }
  if (car == '0') {
    int ret[] = {0, 0, 0, 0, 1, 1, 0};
  }
  if (car == '8') {
    int ret[] = {0, 0, 0, 1, 1, 1, 0};
  }
  if (car == '@') {
    int ret[] = {0, 0, 0, 0, 0, 0, 1};
  }
  if (car == 'H') {
    int ret[] = {0, 0, 0, 1, 0, 0, 1};
  }
  if (car == 'P') {
    int ret[] = {0, 0, 0, 0, 1, 0, 1};
  }
  if (car == 'X') {
    int ret[] = {0, 0, 0, 1, 1, 0, 1};
  }

  // 2nd column of documentation's table
  if (car == '!') {
    int ret[] = {1, 0, 0, 0, 0, 1, 0};
  }
  if (car == '>') {
    int ret[] = {1, 0, 0, 1, 0, 1, 0};
  }
  if (car == '1') {
    int ret[] = {1, 0, 0, 0, 1, 1, 0};
  }
  if (car == '9') {
    int ret[] = {1, 0, 0, 1, 1, 1, 0};
  }
  if (car == 'A') {
    int ret[] = {1, 0, 0, 0, 0, 0, 1};
  }
  if (car == 'I') {
    int ret[] = {1, 0, 0, 1, 0, 0, 1};
  }
  if (car == 'Q') {
    int ret[] = {1, 0, 0, 0, 1, 0, 1};
  }
  if (car == 'Y') {
    int ret[] = {1, 0, 0, 1, 1, 0, 1};
  }

  // 3rd column of documentation's table
  if (car == '\"') {
    int ret[] = {0, 1, 0, 0, 0, 1, 0}; // Does this works ?
  }
  if (car == '*') {
    int ret[] = {0, 1, 0, 1, 0, 1, 0};
  }
  if (car == '2') {
    int ret[] = {0, 1, 0, 0, 1, 1, 0};
  }
  if (car == '"') {
    int ret[] = {0, 1, 0, 1, 1, 1, 0};
  }
  if (car == 'B') {
    int ret[] = {0, 1, 0, 0, 0, 0, 1};
  }
  if (car == 'J') {
    int ret[] = {0, 1, 0, 1, 0, 0, 1};
  }
  if (car == 'R') {
    int ret[] = {0, 1, 0, 0, 1, 0, 1};
  }
  if (car == 'Z') {
    int ret[] = {0, 1, 0, 1, 1, 0, 1};
  }

  // 4th column of documentation's table
  if (car == '!') {
    int ret[] = {1, 1, 0, 0, 0, 1, 0};
  }
  if (car == '+') {
    int ret[] = {1, 1, 0, 1, 0, 1, 0};
  }
  if (car == '3') {
    int ret[] = {1, 1, 0, 0, 1, 1, 0};
  }
  if (car == 'x') {
    int ret[] = {1, 1, 0, 1, 1, 1, 0}; // Unknown
  }
  if (car == 'C') {
    int ret[] = {1, 1, 0, 0, 0, 0, 1};
  }
  if (car == 'K') {
    int ret[] = {1, 1, 0, 1, 0, 0, 1};
  }
  if (car == 'S') {
    int ret[] = {1, 1, 0, 0, 1, 0, 1};
  }
  if (car == '(') {
    int ret[] = {1, 1, 0, 1, 1, 0, 1};
  }

  // 5th column of documentation's table
  if (car == 'x') {
    int ret[] = {0, 0, 1, 0, 0, 1, 0}; // Unknown
  }
  if (car == ',') {
    int ret[] = {0, 0, 1, 1, 0, 1, 0};
  }
  if (car == '4') {
    int ret[] = {0, 0, 1, 0, 1, 1, 0};
  }
  if (car == '<') {
    int ret[] = {0, 0, 1, 1, 1, 1, 0};
  }
  if (car == 'D') {
    int ret[] = {0, 0, 1, 0, 0, 0, 1};
  }
  if (car == 'L') {
    int ret[] = {0, 0, 1, 1, 0, 0, 1};
  }
  if (car == 'T') {
    int ret[] = {0, 0, 1, 0, 1, 0, 1};
  }
  if (car == '\\') {
    int ret[] = {0, 0, 1, 1, 1, 0, 1}; // Does this works ?
  }

  // 6th column of documentation's table
  if (car == '%') {
    int ret[] = {0, 0, 1, 0, 0, 1, 0}; // Does this works ?
  }
  if (car == '-') {
    int ret[] = {0, 0, 1, 1, 0, 1, 0};
  }
  if (car == '5') {
    int ret[] = {0, 0, 1, 0, 1, 1, 0};
  }
  if (car == '=') {
    int ret[] = {0, 0, 1, 1, 1, 1, 0};
  }
  if (car == 'E') {
    int ret[] = {0, 0, 1, 0, 0, 0, 1};
  }
  if (car == 'M') {
    int ret[] = {0, 0, 1, 1, 0, 0, 1};
  }
  if (car == 'U') {
    int ret[] = {0, 0, 1, 0, 1, 0, 1};
  }
  if (car == ')') {
    int ret[] = {0, 0, 1, 1, 1, 0, 1};
  }

  // 7th column of documentation's table
  if (car == '&') {
    int ret[] = {1, 0, 1, 0, 0, 1, 0};
  }
  if (car == '.') {
    int ret[] = {1, 0, 1, 1, 0, 1, 0};
  }
  if (car == '6') {
    int ret[] = {1, 0, 1, 0, 1, 1, 0};
  }
  if (car == '>') {
    int ret[] = {1, 0, 1, 1, 1, 1, 0};
  }
  if (car == 'F') {
    int ret[] = {1, 0, 1, 0, 0, 0, 1};
  }
  if (car == 'N') {
    int ret[] = {1, 0, 1, 1, 0, 0, 1};
  }
  if (car == 'V') {
    int ret[] = {1, 0, 1, 0, 1, 0, 1};
  }
  if (car == '^') {
    int ret[] = {1, 0, 1, 1, 1, 0, 1};
  }

  // 8th column of documentation's table
  if (car == '\'') {
    int ret[] = {1, 1, 1, 0, 0, 1, 0};
  }
  if (car == '/') {
    int ret[] = {1, 1, 1, 1, 0, 1, 0};
  }
  if (car == '7') {
    int ret[] = {1, 1, 1, 0, 1, 1, 0};
  }
  if (car == '?') {
    int ret[] = {1, 1, 1, 1, 1, 1, 0};
  }
  if (car == 'G') {
    int ret[] = {1, 1, 1, 0, 0, 0, 1};
  }
  if (car == 'O') {
    int ret[] = {1, 1, 1, 1, 0, 0, 1};
  }
  if (car == 'W') {
    int ret[] = {1, 1, 1, 0, 1, 0, 1};
  }
  if (car == '_') {
    int ret[] = {1, 1, 1, 1, 1, 0, 1};
  }

  int ret[] = {0, 0, 0, 0, 0, 0, 1}; // return "@" if nothing found
}


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
    return;  // digit should be in 0-3 range.
  }

  // Désactivation des pins
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable

  if (digit == 0) {
    return;
  }

  if (digit == 1) {
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
    return;
  }

  if (digit == 2) {
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
    return;
  }

  if (digit == 3) {
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
    return;
  }
}

void digit_unselect() {
  //  Désactivation des deux digit select
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable
}

void set_data(int a, int b, int c, int d, int e, int f, int g) {
  // writes values a to g into D0 to D6
  // Values a to g should be >= 0 and <= 1
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
  sprintf(s, "%02d%02d", mm, ss); // print to string
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

  ss++;
  if (ss > 59) {
    ss = 0;
    mm++;
  }
  if (mm > 59) {
    mm = 0;
    hh++;
  }
  if (hh > 23) {
    hh = 0;
  }

  analogWrite(LED_BUILTIN, ss * 255 / 60); // ratio=30/255
  delayMicroseconds(500000);// pauses for 0.5 second to display temp

  // Here we print temperature reading

  int* repr = get_representation('#');
  Serial.print(repr[0]);
  Serial.print(repr[1]);
  Serial.print(repr[2]);
  Serial.print(repr[3]);
  Serial.print(repr[4]);
  Serial.print(repr[5]);
  Serial.print(repr[6]);
  Serial.println();
  //set_data(repr[0],repr[1],repr[2],repr[3],repr[4],repr[5],repr[6]);

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

  Serial.begin(115200);
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

  myClock.attach(1.0, updateClockAndDisplay); // 1.0 = 1 second

  //analogWriteFreq(1000); // 1000Hz
  //analogWrite(LED_BUILTIN, 30); // ratio=30/255
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

  // dummy freq on LED_BUILTIN
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(30000);


}
