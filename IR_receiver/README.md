# IR_receiver
## Overview
A sub-project to analyze the signals from genuine remote controller.

## Hardware
* Vishay TSSP4038 (IR sensor module)
* WayinTop ESP32 (Espressif ESP32 Dev Module compatible)

|pin number|connect|
|----------|-------|
|1|GPIO 25|
|2|GND|
|3|3.3V|

## Version
ver.1.0.0 Basic implementation of IR_receiver <br>
ver.1.0.1 Change the logic to distinguish "0" and "1" <br>
ver.1.0.2 Add process to fix "LOW 1300" problem <br>
ver.1.0.3 Add an empty line every 3 lines for readability of results <br>
ver.1.0.4 Display results in hexadecimal representation<br>
ver.1.0.5 Code refactoring with struct of 1 byte <br>
ver.1.1.0 Add error detection (1's complement on 4th ~ 7th byte) <br>
ver.1.1.1 Add error detection (4-bit XOR on 1st ~ 2nd byte) <br>
ver.1.2.0 Store data from LSB to MSB <br>
ver.1.2.1 Adjust error detection <br>