/** 
 *  Current controller configuration:
 *  Right Joystick    --> Right Servo
 *  Right Trigger     --> Right Motor
 *  Left Joystick     --> Left Servo (incomplete, need actual servo)
 *  Left Trigger      --> Left Motor (incomplete, need actual motor)
 */

/**
 * TODO: Debug right servo not working with right joystick
 * TODO: Map horizontal movement of joysticks to act as modifier for strength of motors
 */

#include <XBOXONE.h>
#include <Servo.h>
#include <Wire.h>
#include "ITG3200.h"
#include <ADXL345.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXONE Xbox(&Usb);
ITG3200 gyro;
ADXL345 accel;

//Revision 1.3 (DEV-09947)
#define MAX_RESET 7 //MAX3421E pin 12
#define MAX_GPX   8 //MAX3421E pin 17


// PIN CONSTANTS
#define RIGHTMOTORPIN 6
#define RIGHTSERVOPIN 5
// #define LEFTMOTORPIN value
// #define LEFTSERVOPIN value

// CONTROLLER CONSTANTS
#define TRIGGERMAX 1023
#define TRIGGERMIN 0
#define JOYSTICKMAX 32767
#define JOYSTICKMIN -32768
#define TRIGGERDEADZONE 10
#define JOYSTICKDEADZONE 4000

// MOTOR AND SERVO CONSTANTS
#define ESCMIN 1000
#define ESCMID 1500 // TODO: verify that this value accurately reflects motor mid
#define ESCMAX 2000
#define WPMAX 1000 // TODO: update to accurately reflect max
#define WPMID 1520 // TODO: update to accurately reflect mid
#define WPMIN 2000 // TODO: update to accurately reflect min

Servo RIGHTMOTOR;
Servo RIGHTSERVO;
Servo LEFTMOTOR;
Servo LEFTSERVO;

void setup() { 
  attachAndPin();
  serialConnect();
  setPwmFrequency(RIGHTMOTORPIN, 64); // force pwm at 500Hz
  setPwmFrequency(RIGHTSERVOPIN, 64);
  initGyro();
  Serial.println("WP init done");
  Serial.println("starting 7.5s stop test      |");
    for(int i = 0; i < 30; ++i) {
    delay(250);
    Serial.print("*");
  }
  Serial.println(" | program commencing");
}
void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    rightJoystick();
    Serial.print("       ");
    rightTrigger();
    Serial.print("       ");
    printGyro();
    Serial.print("       ");
    printAccel();
    // leftJoystick();
    // leftTrigger();
    // TODO: format whatever correctly
  }
  delay(12);
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 6) {
      TCCR3B = TCCR3B & 0b11111000 | mode;
    } else {
      TCCR4B = TCCR4B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void attachAndPin() {
  RIGHTMOTOR.attach(RIGHTMOTORPIN);
  RIGHTSERVO.attach(RIGHTSERVOPIN);
  pinMode(MAX_GPX, INPUT);
  pinMode(MAX_RESET, OUTPUT); 
  digitalWrite(MAX_RESET, LOW); 
  delay(20); //wait 20ms 
  digitalWrite(MAX_RESET, HIGH);
  delay(20); //wait 20ms
  Serial.begin(115200);
}

void serialConnect() {
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy a/nd other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXBOX USB Library Started"));
}

void secondAttachAndPin() {
  RIGHTMOTOR.attach(RIGHTMOTORPIN,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  delay(200); // wait
  RIGHTMOTOR.writeMicroseconds(1800); // throttle init
  delay(200); // wait
  RIGHTMOTOR.writeMicroseconds(1500); // stop
  Serial.println("ESC init done");
  RIGHTSERVO.attach(RIGHTSERVOPIN,900,2100);
  RIGHTSERVO.writeMicroseconds(1500);
  delay(200);
  RIGHTSERVO.writeMicroseconds(1500);
}

void leftJoystick() {
  // TODO: When the left servo arrives
  if (Xbox.getAnalogHat(LeftHatY) > JOYSTICKDEADZONE || Xbox.getAnalogHat(LeftHatY) -JOYSTICKDEADZONE) {
    Serial.print("LeftHatY: ");
    double inp = Xbox.getAnalogHat(LeftHatY);
    Serial.print(inp);
    double ret;
    if (Xbox.getAnalogHat(LeftHatY) > 0) ret = map(inp, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMAX);
    else                                 ret = map(inp, JOYSTICKMIN, -JOYSTICKDEADZONE, WPMIN, WPMID);
    Serial.print(", output: ");
    Serial.print(ret);
    // TODO: Write LEFTSERVO
  } else {
    Serial.print("no LeftHatY, resetting to 1520   ");
    // TODO: Write LEFTSERVO
  }
}

void rightJoystick() {
  if (Xbox.getAnalogHat(RightHatY) > JOYSTICKDEADZONE || Xbox.getAnalogHat(RightHatY) < -JOYSTICKDEADZONE) {
    Serial.print(F("RightHatY: "));
    double inp = Xbox.getAnalogHat(RightHatY);
    Serial.print(inp);
    double ret;
    if (Xbox.getAnalogHat(RightHatY) > 0) ret = map(inp, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMIN);
    else                                  ret = map(inp, JOYSTICKMIN, -JOYSTICKDEADZONE, WPMAX, WPMID);
    Serial.print(", output: ");
    Serial.print(ret);
    RIGHTSERVO.writeMicroseconds(ret);
    } else {
      Serial.print("no RightHatY, resetting to 1520   ");
      RIGHTSERVO.writeMicroseconds(1520);
  }
}

void rightTrigger() {
  if(Xbox.getButtonPress(R2) > TRIGGERDEADZONE) {
    double inp = Xbox.getButtonPress(R2);
    double ret = map(inp, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);
    Serial.print("R2Trigger: ");
    Serial.print(inp);
    Serial.print(", output: ");
    Serial.print(ret);
    RIGHTMOTOR.writeMicroseconds(ret);
    // TODO: Bumper to make value negative
  }
  else {
    Serial.print("no R2 Trigger, resetting to 1500");
    RIGHTMOTOR.writeMicroseconds(1500);
  }
  Serial.println();
}

void leftTrigger() {
  if(Xbox.getButtonPress(L2) > TRIGGERDEADZONE) {
    // TODO: When the left motor arrives
    double inp = Xbox.getButtonPress(R2);
    double ret = map(inp, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);
    Serial.print("L2Trigger: ");
    Serial.print(inp);
    Serial.print(", output: ");
    Serial.print(ret);
    // TODO: Write LEFTMOTOR
    // TODO: Bumper to make value negative
  }
  else {
    Serial.print("no L2 Trigger, resetting to 1500");
    // TODO: Write LEFTMOTOR
  }
}

void initGyro() {
  gyro.init();
  Serial.println("Calibrating Gyro");
  gyro.zeroCalibrate(200, 10);
  Serial.println("Gyro calibrated");
}

void printGyro() {
  int16_t x, y, z;
  gyro.getXYZ(&x, &y, &z);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);
}

void initAccelerometer() {
  accel.powerOn();
  //set activity/ inactivity thresholds (0-255)
  accel.setActivityThreshold(75); //62.5mg per increment
  accel.setInactivityThreshold(75); //62.5mg per increment
  accel.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  accel.setActivityX(1);
  accel.setActivityY(1);
  accel.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  accel.setInactivityX(1);
  accel.setInactivityY(1);
  accel.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  accel.setTapDetectionOnX(0);
  accel.setTapDetectionOnY(0);
  accel.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  accel.setTapThreshold(50); //62.5mg per increment
  accel.setTapDuration(15); //625us per increment
  accel.setDoubleTapLatency(80); //1.25ms per increment
  accel.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  accel.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  accel.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  accel.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  accel.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  accel.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  accel.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  accel.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  accel.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  accel.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  accel.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  accel.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  accel.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

void printAccel() {
  double xyz[3];
  accel.getAcceleration(xyz);
  Serial.print("x-accel: ");
  Serial.print(xyz[0]);
  Serial.print("  y-accel: ");
  Serial.print(xyz[1]);
  Serial.print("  z-accel: ");
  Serial.println("xyz[2]");
}
