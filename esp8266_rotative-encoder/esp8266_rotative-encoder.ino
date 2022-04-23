const int RGBLED_RED   = 5;
const int RGBLED_GREEN = 4;
const int RGBLED_BLUE  = 0;

int switchPin = 12;                        // button pin

int pinA = 2;                              // Rotary encoder Pin A = CLK
int pinB = 14;                              // Rotary encoder Pin B = DT

const int LEDBLINK_PIN  = 13;



enum PinAssignments {
encoderPinA = 2,
encoderPinB = 14,
clearButton = 12
};

volatile unsigned int encoderPos = 0;
unsigned int lastReportedPos = 1;

boolean A_set = false;
boolean B_set = false;

// Interrupt on A changing state
ICACHE_RAM_ATTR void doEncoderA() {
  // Test transition
  A_set = digitalRead(encoderPinA) == HIGH;
  // and adjust counter + if A leads B
  encoderPos += (A_set != B_set) ? +1 : -1;
}

// Interrupt on B changing state
ICACHE_RAM_ATTR void doEncoderB() {
  // Test transition
  B_set = digitalRead(encoderPinB) == HIGH;
  // and adjust counter + if B follows A
  encoderPos += (A_set == B_set) ? +1 : -1;
}

void setup() {
  pinMode(encoderPinA, INPUT);
  Serial.begin(9600);
  pinMode(encoderPinB, INPUT);
  pinMode(clearButton, INPUT);
  digitalWrite(encoderPinA, HIGH);  // turn on pull-up resistor
  digitalWrite(encoderPinB, HIGH);  // turn on pull-up resistor
  digitalWrite(clearButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

}


void loop() {
  if (lastReportedPos != encoderPos) {
    Serial.print("Index:");
    Serial.print(encoderPos, DEC);
    Serial.println();
    lastReportedPos = encoderPos;
  }
  if (digitalRead(clearButton) == LOW)  {
    encoderPos = 0;
  }
//  analogWrite(RGBLED_RED, 0);
//  analogWrite(RGBLED_GREEN, 255);
//  analogWrite(RGBLED_BLUE, 255);
//  digitalWrite(LEDBLINK_PIN, LOW);
//  delayMicroseconds(250000);
//  digitalWrite(LEDBLINK_PIN, HIGH);
//  delayMicroseconds(250000);
  
}
