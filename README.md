# Moto-Gymkhana counter #
The telemetry system is used to measure the time between start and finish in circuit racing
---
## Board ##
ESP8266
## Dependens ##
1. ArduinoCore
2. ESPAsyncTCP 
---
## Build system ##
platformio
---
## Use pins: ##
1. D1 - sensor (NPN laser sensor)
2. D2 - reset button (NO button)
3. D3 - led indicator
4. D8, D7, D5 - connection to MAX7219 (SPI)
---
## Functional ##
1. WiFi update
2. Hard restart when holded reset button
3. Data transfer via wifi
---
## Links ##
https://grabcad.com/library/telemetry-system-1
https://platformio.org/