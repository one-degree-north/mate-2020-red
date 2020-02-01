                                                                                                                                                                                        /** 
 *  Current controller configuration:
 *  Right Joystick                --> Right Servo                       --> (incomplete, need actual servo)
 *  Right Trigger                 --> Right Motor                       --> (incomplete, need actual motor)
 *  Left Joystick                 --> Left Servo
 *  Left Trigger                  --> Left Motor
 *  D-Pad Up                      --> Back Motor Vertical Push          --> (incomplete, need actual motor)
 *  D-Pad Down                    --> Back Motor Vertical Pull          --> (incomplete, need actual motor)
 *  D-Pad Right                   --> Back Servo Clockwise              --> (incomplete, need actual motor)
 *  D-Pad Left                    --> Back Servo Counter-Clockwise      --> (incomplete, need actual motor)
 */

/**
 * TODO: Debug right servo not working with right joystick
 * TODO: Map horizontal movement of joysticks to act as modifier for strength of motors
 */

// LIBRARY IMPORTS
#include <XBOXONE.h>                        // Library to use the XBox Controller
#include <Servo.h>                          // Library to use servos and motors
#include <Wire.h>                           // Library to be able to communicate with I2C / TWI devices
#include "ITG3200.h"                        // Library to use gyroscopic functions of an accelerometer
#include <ADXL345.h>                        // Library to use accelerometer functions

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif


// OBJECTS
USB Usb;                                    // Objoct to establish USB connection between Arduino and computer
XBOXONE Xbox(&Usb);                         // Object to use USB connection with XBox controller
ITG3200 gyro;                               // Object to add gyroscopic functionality
ADXL345 accel;                              // Object to add accelerometer functionality
Servo RIGHTMOTOR;                           // Object to control the right motor
Servo RIGHTSERVO;                           // Object to control the right servo
Servo LEFTMOTOR;                            // Object to control the left motor
Servo LEFTSERVO;                            // Object to control the left servo

//Revision 1.3 (DEV-09947)
#define MAX_RESET 7 //MAX3421E pin 12
#define MAX_GPX   8 //MAX3421E pin 17


// PIN CONSTANTS
#define LEFTMOTORPIN 6                      // Pin the LEFTMOTOR to a PWM pin
#define LEFTSERVOPIN 5                      // Pin the LEFTSERVO to a PWM pin
// #define RIGHTMOTORPIN value
// #define RIGHTSERVOPIN value

// CONTROLLER CONSTANTS
#define TRIGGERMAX 1023                     // The maximum value a controller trigger returns when fully pressed
#define TRIGGERMIN 0                        // The minimum value a controller trigger returns when fully released
#define JOYSTICKMAX 32767                   // The maximum value a controller joystick returns when fully up
#define JOYSTICKMIN -32768                  // The minimum value a controller joystick returns when fully down
#define TRIGGERDEADZONE 10                  // The value that needs to be exceeded to start sending power to the motor
#define JOYSTICKDEADZONE 4000               // The value that needs to be exceeded to start sending power to the servo

// MOTOR AND SERVO CONSTANTS
#define ESCMIN 1000                         // The minimum value of writeMicroseconds for the motor to be fully powered in reverse
#define ESCMID 1500                         // The default value of writeMicroseconds for the motor to remain still
#define ESCMAX 2000                         // The maximum value of writeMicroseconds for the motor to be fully powered
#define WPMIN 1000                          // The minimum value of writeMicroseconds for the servo to be in min position
#define WPMID 1520                          // The default value of writeMicroseconds for the servo to be in equilibrium position
#define WPMAX 2000                          // The maximum value of writeMicroseconds for the servo to be in max position




// SETUP

void setup() { 
  attachAndPin();
  serialConnect();
  setPwmFrequency(LEFTMOTORPIN, 64); // force pwm at 500Hz
  setPwmFrequency(LEFTSERVOPIN, 64);
  secondAttachAndPin();
  stopTest();
}

void attachAndPin() {
  LEFTMOTOR.attach(LEFTMOTORPIN);
  LEFTSERVO.attach(LEFTSERVOPIN);
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
  Serial.println("Serial connected");
  Serial.println(F("\r\nXBOX USB Library Started"));
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
  Serial.print("Set pin ");
  Serial.print(pin);
  Serial.print(" to ");
  Serial.println(divisor);
}

void secondAttachAndPin() {
  LEFTMOTOR.attach(LEFTMOTORPIN,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  delay(200); // wait
  LEFTMOTOR.writeMicroseconds(1800); // throttle init
  delay(200); // wait
  LEFTMOTOR.writeMicroseconds(1500); // stop
  Serial.println("LEFTMOTOR init done");
  LEFTSERVO.attach(LEFTSERVOPIN,900,2100);
  LEFTSERVO.writeMicroseconds(1500);
  delay(200);
  LEFTSERVO.writeMicroseconds(1500);
  Serial.println("LEFTSERVO init done");
}

/** [DEPRECATED | DO NOT USE] initializes the gyro
 * 
 */
void initGyro() {
  gyro.init();
  Serial.println("Calibrating Gyro");
  gyro.zeroCalibrate(200, 10);
  Serial.println("Gyro calibrated");
}

/** [DEPRECATED | DO NOT USE] initializes the accelerometer
 * 
 */
void initAccelerometer() {
  Serial.println("Initializing accelerometer");
  
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

  Serial.println("Accelerometer initialized");
}

void stopTest() {
  Serial.println("starting 7.5s stop test       ||");
  for(int i = 0; i < 30; ++i) {
    delay(250);
    Serial.print("*");
  }
  Serial.println(" | program commencing");
}













// Driving phase

void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    leftJoystick();
    Serial.print("\t\t\t");
    leftTrigger();
    Serial.println();
  }
  else {
    Serial.println("ERROR: No controller input\t\t\t");
  }
  
  delay(12);
}



/** Maps the right joystick to the right servo
 *    The right joystick powers the right servo. This design choice was made because
 *    the right joystick returns an analog value based on its position from its equilibrium,
 *    which allows us to develop precision when driving the robot. Moving the analog stick
 *    up moves the servo to tilt the motor in a way to push the robot down. Likewise,
 *    moving the analog stick down tilts the motor in a way to push the robot up. This
 *    mirrors the centre stick of a pilot.
 */
void rightJoystick() {
  double joystick_input = Xbox.getAnalogHat(RightHatY);
  Serial.print("RightHatY: ");
  Serial.print(joystick_input);
  if (joystick_input > JOYSTICKDEADZONE || joystick_input < -1 * JOYSTICKDEADZONE) {
    double power;
    if(joystick_input > 0) power = map(joystick_input, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMAX);
    else                   power = map(joystick_input, JOYSTICKMIN, -1*JOYSTICKDEADZONE, WPMIN, WPMID);

    Serial.print(",\t\toutput:");
    Serial.print(power);

    // RIGHTSERVO.writeMicroseconds(power);
  }
  else {
    Serial.print(", no output");
    // RIGHTSERVO.writeMicroseconds(1520);
  }
}

/** Maps the left joystick to the left servo
 *    The left joystick controls the left servo. This design choice was made because
 *    the left joystick returns an analog value based on its position from its equilibrium,
 *    which allows us to develop precision when driving the robot. Moving the analog stick
 *    up moves the servo to tilt the motor in a way to push the robot down. Likewise,
 *    moving the analog stick down tilts the motor in a way to push the robot up. This
 *    mirrors the centre stick of a pilot.
 */
void leftJoystick() {
  double joystick_input = Xbox.getAnalogHat(LeftHatY);
  Serial.print("LeftHatY: ");
  Serial.print(joystick_input);
  
  if (joystick_input > JOYSTICKDEADZONE || joystick_input < -1 * JOYSTICKDEADZONE) {
    double power;
    if(joystick_input > 0) power = map(joystick_input, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMAX);
    else                   power = map(joystick_input, JOYSTICKMIN, -1*JOYSTICKDEADZONE, WPMIN, WPMID);

    Serial.print(",\t\toutput:");
    Serial.print(power);

    LEFTSERVO.writeMicroseconds(power);
  }
  else {
    Serial.print("no output");
    LEFTSERVO.writeMicroseconds(1520);
  }
}

/** Maps the right trigger to the right motor
 *    The right trigger powers the right motor. This design choice was made because
 *    the right trigger returns an analog value based on how hard it is being pressed,
 *    which allows us to develop precision while driving the robot. Pressing the left
 *    bumper reverses the motor, allowing the robot to move backwards. However, the
 *    driver must be careful, as incorrect use of the bumper may jolt the robot and 
 *    disorient the driver.
 */
void rightTrigger() {
  double trigger_input = Xbox.getButtonPress(R2);
  Serial.print("R2Trigger: ");
  Serial.print(trigger_input);
  if(trigger_input > TRIGGERDEADZONE) {
    double power = map(trigger_input, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);

    // RIGHTMOTOR.writeMicroseconds(power);

    Serial.print(",\t\toutput: ");
    Serial.print(power);
  }
  else {
    Serial.print(", no output");
  }
}

/** Maps the left trigger to the left motor
 *    The left trigger powers the left motor. This design choice was made because
 *    the left trigger returns an analog value based on how hard it is being pressed,
 *    which allows us to develop precision while driving the robot. Pressing the
 *    left bumper reverses the motor, allowing the robot to move backwards. However,
 *    the driver must be careful, as incorrect use of the bumper may jolt the robot
 *    and disorient the driver.
 */
void leftTrigger() {
  double trigger_input = Xbox.getButtonPress(L2);
  Serial.print("L2Trigger: ");
  Serial.print(trigger_input);
  if(trigger_input > TRIGGERDEADZONE) {
    double power = map(trigger_input, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);

    LEFTMOTOR.writeMicroseconds(power);
    //TODO: Bumper take make value negative

    Serial.print(",\t\toutput: ");
    Serial.print(power);
  }
  else {
    Serial.print(", no output");
    LEFTMOTOR.writeMicroseconds(1500);
  }
}

/** [DEPRECATED | DO NOT USE] Prints the values of the gyroscope
 *    This allows us to understand the orientation of the robot underwater
 *    and make necessary adjustments during the driving phase. 
 */
void printGyro() {
  int16_t x, y, z;
  gyro.getXYZ(&x, &y, &z);
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(z);
}

/** [DEPRECATED | DO NOT USE] Prints the values of the accelerometer
 *    This allows us to understand how fast the robot is travelling when it's
 *    out of sight. It is necessary during the driving phase so we don't
 *    unintentianally crash into anything, which could potentially be
 *    disastrous to the functionality of the robot's components.
 */
void printAccel() {
  double xyz[3];
  accel.getAcceleration(xyz);
  Serial.print("x-accel: ");
  Serial.print(xyz[0]);
  Serial.print("  y-accel: ");
  Serial.print(xyz[1]);
  Serial.print("  z-accel: ");
  Serial.print(xyz[2]);
}
