# esp8266
playing with esp8266

## Infos

### On-board LED

Most of the ESP8266 development boards have a built-in LED. This LED is usually connected to GPIO2.

## Issues

### COM port not recognized

Mostly bad USB cable.
Install drivers : https://github.com/nodemcu/nodemcu-devkit/tree/master/Drivers

### 5V output

No 5V regulated ouput on NodeMCU, though you can use VIN port which outputs what is powering the chip (I've 4.89V).
