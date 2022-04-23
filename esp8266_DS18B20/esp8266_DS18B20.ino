/*

C.BRESSY 2022.04.23

Libraries to make DS18B20 work :
DallasTemperature by Miles Burton
One Wire Library 

SENSOR_RESOLUTION :
9 bits: increments of 0.5C, 93.75ms to measure temperature;
10 bits: increments of 0.25C, 187.5ms to measure temperature;
11 bits: increments of 0.125C, 375ms to measure temperature;
12 bits: increments of 0.0625C, 750ms to measure temperature.

*/
#include <OneWire.h>
#include <DallasTemperature.h>


// Sensor input pin
#define DATA_PIN 5
// How many bits to use for temperature values: 9, 10, 11 or 12
#define SENSOR_RESOLUTION 9
// Index of sensors connected to data pin, default: 0
#define SENSOR_INDEX 0


OneWire oneWire(DATA_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

unsigned int i = 0;

void setup() {
  sensors.begin();  // Start up the library
  sensors.getAddress(sensorDeviceAddress, 0);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
  Serial.begin(9600);
}

void loop() {

  if(i >= 999){i=0;}
  
  Serial.print(i);
  // Send the command to get temperatures
  sensors.requestTemperatures();
  // Measurement may take up to 750ms

  float temperatureInCelsius = sensors.getTempCByIndex(SENSOR_INDEX);
  float temperatureInFahrenheit = sensors.getTempFByIndex(SENSOR_INDEX);

  Serial.print("Temperature: ");
  Serial.print(temperatureInCelsius, 4);
  Serial.print((char)176);//shows degrees character
  Serial.println("C");
  
  delay(1000);

  i = i + 1;
}
