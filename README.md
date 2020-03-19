# mate-red

The repository for One Degree North's Red team. The technical paper can be found [here](https://docs.google.com/document/d/11K5dcY7s_mxuk-Gu6wQhp6wE9jx9zbS2zczkxVFlV6A/). 

### Code

bruh/bruh.ino -- Main code with controller configuration

Everything else -- practically useless

##### Required Libraries:


###### Libaries that do not need to be installed (Included in Arduino IDE):

Servo.h - **Servo**

Wire.h - **Wire**


###### Libraries that need to be installed:

XBOXONE.h - [**USB Host Shield**](https://github.com/felis/USB_Host_Shield_2.0)

TimeLib.h - [**Time**](https://github.com/PaulStoffregen/Time)

### Hardware

- Control Station
  - Raspberry Pi 4 B 2GB
  - *Monitor - TBD*
  - *Keyboard and mouse - TBD*

- Motor and I/O Control: Arduino Mega 2560 with:
  - USB Host - USB Host Shield Keyes
  - ESC - Blue 20A Basic ESC *x3*

- Sensors
  - Orientation Sensor Processing and Communication: Adafruit Trinket M0

- Motors and Servo
  - Blue Robotics T100 thruster *x3*
  - KST BLS662WP *x2*
  - Goolsky DSSERVO DS3225 25KG Digital Servo *x2*
  
