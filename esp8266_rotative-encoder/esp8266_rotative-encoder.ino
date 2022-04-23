
int clkInterruptPin = 14; // GPIO14, pin D5
int dataPin = 12;         // GPIO2,  pin D4

volatile unsigned int encoderPos = 0;

unsigned int loopCounter = 0;

ICACHE_RAM_ATTR void interrupt_function()
{
    encoderPos += digitalRead(dataPin) == HIGH ? 1 : -1;
}

void setup()
{

    Serial.begin(9600);

    pinMode(clkInterruptPin, INPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clkInterruptPin, HIGH); // turn on pull-up resistor
    digitalWrite(dataPin, HIGH);         // turn on pull-up resistor

    
    attachInterrupt(digitalPinToInterrupt(clkInterruptPin), interrupt_function, CHANGE);
}

void loop()
{
    Serial.print(loopCounter);
    Serial.print("encoderPos:");
    Serial.print(encoderPos, DEC);
    Serial.println();
    delayMicroseconds(250000);
    loopCounter++;
}
