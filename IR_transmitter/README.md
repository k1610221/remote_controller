# IR_transmitter
## Overview
A sub-project to emulate a remote controller.

## Hardware
* LENOO LIR053 (IR LED)
* WayinTop ESP32 (Espressif ESP32 Dev Module compatible)
* 100 ohm resistor

Connection:
```
GND --- IR LED --- 100R --- GPIO25
```
## Version
ver.2.0.0 Basic implementation for confirming the usage of API <br>
ver.2.1.0 Sending 7-byte signal with library <br>
ver.2.2.0 Sending whole command (but not working yet) <br>
ver.2.2.1 Minor bug fix (Yes it's working now!) <br>
ver.2.2.2 Add some constraints <br>