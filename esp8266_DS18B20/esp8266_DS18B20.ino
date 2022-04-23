/*

C.BRESSY 2022.04.23

Libraries to make DS18B20 work :
DallasTemperature by Miles Burton
One Wire Library 

*/
#include <OneWire.h>
#include <DallasTemperature.h>


// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 16

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

unsigned int i = 0;

void setup() {
  sensors.begin();  // Start up the library
  Serial.begin(9600);
}

void loop() {

  if(i >= 999){i=0;}
  
  Serial.print(i);
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print(" Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);//shows degrees character
  Serial.println("F");
  
  delay(500);

  i = i + 1;
}
