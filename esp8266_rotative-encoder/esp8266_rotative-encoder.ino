
int clkInterruptPin = 14; // GPIO14, pin D5
int dataPin = 12;         // GPIO12, pin D6

volatile unsigned int encoderPos = 0;

unsigned int loopCounter = 0;

ICACHE_RAM_ATTR void interrupt_function()
{
    encoderPos += digitalRead(dataPin) == HIGH ? 1 : -1;
}

void setup()
{

    Serial.begin(9600);

    pinMode(dataPin, INPUT_PULLUP);
    pinMode(clkInterruptPin, INPUT);
    
    
    attachInterrupt(digitalPinToInterrupt(clkInterruptPin), interrupt_function, RISING);
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
