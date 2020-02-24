# mate-red
## MATE Red Repo

The Mate Red Repo is divided into designs and code

* Software - The code for Arduino, along with the Driver interface

* Hardware - Contains hardware info and electric schamatics



### Code:

bruh/bruh.ino -- Main code with controller configuration

Everything else -- practically useless

##### Required Libraries:


###### Libaries that do not need to be installed (Included in Arduino IDE):

Servo.h - **Servo**

Wire.h - **Wire**


###### Libraries that need to be installed:

XBOXONE.h - [**USB Host Shield**](https://github.com/felis/USB_Host_Shield_2.0)

TimeLib.h - *Time*

### Hardware:

* The Schematics folder has the electric schematics in it

Our current hardware is:
Control Station: Raspberry Pi 4 B 2GB
Motor and I/O Control: Arduino Mega 2560
  USB Host - USB Host Shield Keyes
  ESC - Blue 20A Basic ESC x 3
Orientation Sensor Processing and Communication: Adafruit Trinket M0
  Orientation Sensor: 9DOF BNO055 Adafruit Breakout
