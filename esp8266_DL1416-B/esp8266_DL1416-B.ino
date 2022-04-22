/*********
* C.BRESSY 22.04.2022

* Tests afficheur DL1416 B

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

TW (Time to Write) dans la doc a 25 degrés c'est minimum 0,25 microsecondes

D0 à D6 il faut attendre 50 nanosecondes avant de baisser le write

Chip enable on peut 



F5QiRNX1rCf9iqNaYg

*********/

#define BUILT_IN_BLINK_ENABLED 1
#define NUM_DIGIT 4
      


// Définition des pins GPIO
//int GPIO_CHIP_ENABLE = 12;
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

unsigned char count=0;
unsigned char x=0;
char s[NUM_DIGIT+1]="0000";

void chip_enable(){
    //digitalWrite(GPIO_CHIP_ENABLE, HIGH); // sets the pin on
}

void chip_disable(){
    //digitalWrite(GPIO_CHIP_ENABLE, LOW); // sets the pin off
}

void write_enable(){
    digitalWrite(GPIO_WRITE, HIGH); // Enable
    delayMicroseconds(50);      // pauses for 50 microseconds
}

void write_disable(){
    digitalWrite(GPIO_WRITE, LOW); // Disable
    delayMicroseconds(50);      // pauses for 50 microseconds
}

void digit_select(int digit){
  if (digit > 3 || digit < 0){
    return; // digit should be in 0-3 range.
  }
  
  // Désactivation des pins
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable

  if (digit == 0){
    digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  }

  if (digit == 1){
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
  }

  if (digit == 2){
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
  }

  if (digit == 3){
    digitalWrite(GPIO_DIGIT_SELECT_A0, HIGH); // Enable
    digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH); // Enable
  }
}

void digit_unselect(){
  //  Désactivation des deux digit select
  digitalWrite(GPIO_DIGIT_SELECT_A0, LOW); // Disable
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW); // Disable
}

void set_data(int a, int b, int c, int d, int e, int f, int g){
  digitalWrite(GPIO_D0, a);
  digitalWrite(GPIO_D1, b);
  digitalWrite(GPIO_D2, c);
  digitalWrite(GPIO_D3, d);
  digitalWrite(GPIO_D4, e);
  digitalWrite(GPIO_D5, f);
  digitalWrite(GPIO_D6, g);
}

void setup() {
    
   
  pinMode(LED_BUILTIN, OUTPUT);


  // initialize GPIOs as outputs.
  //pinMode(GPIO_CHIP_ENABLE, OUTPUT);
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
  
  ESP.wdtDisable();
}


void loop_old() {
  digitalWrite(GPIO_DIGIT_SELECT_A1, LOW);

  delayMicroseconds(250000);
  digitalWrite(GPIO_DIGIT_SELECT_A1, HIGH);
  delayMicroseconds(250000);
}

// the loop function runs over and over again forever
void loop() {
    
    
#if BUILT_IN_BLINK_ENABLED
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(25000);
    digitalWrite(LED_BUILTIN, LOW);
#endif
    
#if 1
  sprintf(s,"%04d",count);
    
  for(int digit=0;digit<NUM_DIGIT;digit++) {
      chip_disable();
      digit_select(digit);
      x = s[NUM_DIGIT-1-digit];
      set_data(x&0x40?1:0,x&0x20?1:0,x&0x10?1:0,x&0x08?1:0,x&0x04?1:0,x&0x02?1:0,x&0x01?1:0);
      write_disable();
      delayMicroseconds(50);      // pauses for 50 microseconds
      write_enable();
      digit_unselect();
      chip_enable();

      delayMicroseconds(50);      // pauses for 50 microseconds
  }  
    
  // update counter (infinite loop 0 to 99)
  count = count<9999 ? count+1 : 0;
  delayMicroseconds(200000);      // pause for 200 ms

#else
    
  // Digit 0
  chip_disable();
  digit_select(0);
  x = s[3];
  set_data(x&0x40?1:0,x&0x20?1:0,x&0x10?1:0,x&0x08?1:0,x&0x04?1:0,x&0x02?1:0,x&0x01?1:0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 1
  chip_disable();
  digit_select(1);
  x = s[2];
  set_data(x&0x40?1:0,x&0x20?1:0,x&0x10?1:0,x&0x08?1:0,x&0x04?1:0,x&0x02?1:0,x&0x01?1:0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 2
  chip_disable();
  digit_select(2);
  x = s[1];
  set_data(x&0x40?1:0,x&0x20?1:0,x&0x10?1:0,x&0x08?1:0,x&0x04?1:0,x&0x02?1:0,x&0x01?1:0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds

  // Digit 3
  chip_disable();
  digit_select(3);
  x = s[0];
  set_data(x&0x40?1:0,x&0x20?1:0,x&0x10?1:0,x&0x08?1:0,x&0x04?1:0,x&0x02?1:0,x&0x01?1:0);
  write_disable();
  delayMicroseconds(50);      // pauses for 50 microseconds
  write_enable();
  digit_unselect();
  chip_enable();

  delayMicroseconds(50);      // pauses for 50 microseconds
#endif
    

}
