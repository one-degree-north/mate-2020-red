/* 
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
#include <LiquidCrystal.h>
#include <TimeLib.h>


// OBJECTS
USB Usb;                                    // Objoct to establish USB connection between Arduino and computer
XBOXONE Xbox(&Usb);                         // Object to use USB connection with XBox controller
ITG3200 gyro;                               // Object to add gyroscopic functionality
ADXL345 accel;                              // Object to add accelerometer functionality
Servo RIGHTMOTOR;                           // Object to control the right motor
Servo RIGHTSERVO;                           // Object to control the right servo
Servo LEFTMOTOR;                            // Object to control the left motor
Servo LEFTSERVO;                            // Object to control the left servo
Servo BACKMOTOR;                            // Object to control the rear motor
Servo BACKSERVO;                            // Object to control the rear servo

//Revision 1.3 (DEV-09947)
#define MAX_RESET 7                         // MAX3421E pin 12
#define MAX_GPX   8                         // MAX3421E pin 17
#define TIMESYNCLEDPIN 20                       // Pin to display whether clock is synced

// PIN CONSTANTS
#define LEFTMOTORPIN 6                      // Pin the LEFTMOTOR to a PWM pin
#define LEFTSERVOPIN 5                      // Pin the LEFTSERVO to a PWM pin
// #define RIGHTMOTORPIN value
// #define RIGHTSERVOPIN value
// #define BACKMOTORPIN value
// #define BACKSERVOPIN value

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
#define WPMIN 900                           // The minimum value of writeMicroseconds for the servo to be in min position
#define WPMID 1500                          // The default value of writeMicroseconds for the servo to be in equilibrium position
#define WPMAX 2100                          // The maximum value of writeMicroseconds for the servo to be in max position




// SETUP

void setup() { 
  Serial.begin(115200);
  attachAndPin();
  serialConnect();
  setPwmFrequency(LEFTMOTORPIN, 64); // force pwm at 500Hz
  setPwmFrequency(LEFTSERVOPIN, 64);
  secondAttachAndPin();
  stopTest();
  //Serial.println(digitalClockDisplay())
}
/** Attaches motors and servos to their respective pins on the Arduino
 *    This is needed to be able to send power to each motor and servo. This should only
 *    be called once; it is not needed to set the pins repeatedly.
 */
void attachAndPin() {
  LEFTMOTOR.attach(LEFTMOTORPIN);
  LEFTSERVO.attach(LEFTSERVOPIN);
  // RIGHTMOTOR.attach(RIGHTMOTORPIN);
  // RIGHTSERVO.attach(RIGHTSERVOPIN);
  // BACKMOTOR.attach(BACKMOTORPIN);
  // BACKSERVO.attach(BACKSERVOPIN);
  pinMode(MAX_GPX, INPUT);
  pinMode(MAX_RESET, OUTPUT); 
  digitalWrite(MAX_RESET, LOW); 
  delay(20); //wait 20ms 
  digitalWrite(MAX_RESET, HIGH);
  delay(20); //wait 20ms
  Serial.begin(115200);
}

/** Connects to the Serial
 *    This enables the Arduino to send information to the computer. This is needed so
 *    that the driver interface can display the information it receives. This should
 *    only be called once; you do not need to setup the serial once it has already been
 *    set up.
 */
void serialConnect() {
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy a/nd other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.println("Serial connected");
  Serial.println(F("\r\nXBOX USB Library Started"));
}

/** Used on PWM Digital pins to emulate the electricity output of Analog pins
 *    Without enabling this functionality, the motors and servos would be unable to function
 *    correctly. Likewise, this only needs to be called once (when setting up).
 */
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

/** Pins the motors and servos for validation
 *    It sends output to the pinned servos and motors just to validate that the motors
 *    and servos have been properly attached to pins.
 */
void secondAttachAndPin() {
  motorPin(LEFTMOTOR, LEFTMOTORPIN);
  servoPin(LEFTSERVO, LEFTSERVOPIN);
  // motorPin(RIGHTMOTOR, RIGHTMOTORPIN);
  // servoPin(RIGHTSERVO, RIGHTSERVOPIN);
  // motorPin(BACKMOTOR, BACKMOTORPIN);
  // servoPin(BACKSERVO, BACKSERVOPIN);
}

/** Individually pins motors
 *    Used for convenience, rather than having to retype this block of code multiple times
 *    for each motor.
 */
void motorPin(Servo motor, int pin) {
  motor.attach(pin, ESCMIN, ESCMAX);            // (pin, min pulse width, max pulse width in microseconds)
  delay(200);
  motor.writeMicroseconds(1800);                // throttle init
  delay(200);
  motor.writeMicroseconds(ESCMID);              // stop
  Serial.print("Motor setup to pin ");
  Serial.print(pin);
  Serial.println(" complete.");
}

/** Individually pins servos
 *    Used for convenience, rather than having to retype this block of code multiple times
 *    for each servo.
 */
void servoPin(Servo servo, int pin) {
  servo.attach(pin, WPMIN, WPMAX);
  servo.writeMicroseconds(WPMID);
  delay(200);
  servo.writeMicroseconds(WPMID);
  Serial.print("Servo setup to pin ");
  Serial.print(pin);
  Serial.println(" complete.");
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
    Serial.print("#");
  }
  Serial.println(" | program commencing");
  Serial.println("setup completed");
}













// Driving phase

void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    leftJoystick();
    leftTrigger();
    Serial.println();
  }
  else {
    Serial.println("ERROR: No controller input");
  }
  if (Serial.available())            processSyncMessage(); 
  if (timeStatus()!= timeNotSet)     digitalClockDisplay();  
  if (timeStatus() == timeSet)       digitalWrite(TIMESYNCLEDPIN, HIGH); // LED on if synced
  else                               digitalWrite(TIMESYNCLEDPIN, LOW);  // LED off if needs refresh

  delay(15);
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
  Serial.print("RightHatY:");
  Serial.print(joystick_input);
  if (joystick_input > JOYSTICKDEADZONE || joystick_input < -1 * JOYSTICKDEADZONE) {
    double power;
    if(joystick_input > 0) power = map(joystick_input, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMAX);
    else                   power = map(joystick_input, JOYSTICKMIN, -1*JOYSTICKDEADZONE, WPMIN, WPMID);

    Serial.print("|");
    Serial.print(power);

    // RIGHTSERVO.writeMicroseconds(power);
  }
  else {
    Serial.print("|0");
    // RIGHTSERVO.writeMicroseconds(WPMID);
  }
  Serial.print(">");
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
  Serial.print("LeftHatY:");
  Serial.print(joystick_input);
  
  if (joystick_input > JOYSTICKDEADZONE || joystick_input < -1 * JOYSTICKDEADZONE) {
    double power;
    if(joystick_input > 0) power = map(joystick_input, JOYSTICKDEADZONE, JOYSTICKMAX, WPMID, WPMAX);
    else                   power = map(joystick_input, JOYSTICKMIN, -1*JOYSTICKDEADZONE, WPMIN, WPMID);

    Serial.print("|");
    Serial.print(power);

    LEFTSERVO.writeMicroseconds(power);
  }
  else {
    Serial.print("|0");
    LEFTSERVO.writeMicroseconds(WPMID);
  }
  Serial.print(">");
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
  Serial.print("R2Trigger:");
  Serial.print(trigger_input);
  if(trigger_input > TRIGGERDEADZONE) {
    double power = map(trigger_input, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);

    // RIGHTMOTOR.writeMicroseconds(power);

    Serial.print("|");
    Serial.print(power);
  }
  else {
    Serial.print("|0");
  }
  Serial.print(">");
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
  Serial.print("L2Trigger:");
  Serial.print(trigger_input);
  if(trigger_input > TRIGGERDEADZONE) {
    double power = map(trigger_input, TRIGGERDEADZONE, TRIGGERMAX, ESCMIN, ESCMAX);

    LEFTMOTOR.writeMicroseconds(power);
    //TODO: Bumper take make value negative

    Serial.print("|");
    Serial.print(power);
  }
  else {
    Serial.print("|0");
    LEFTMOTOR.writeMicroseconds(ESCMID);
  }
  Serial.print(">");
}

/** Maps the directional pad to the back motor and servo
 *    The reasoning for this is that the back motors and servos do not require as much 
 *    precision compared to the ones located on the sides. 
 */
void directionalPad() {
  dpadMotor();
  dpadServo();
}

/** Maps the up and down buttons of the directional pad to the motor
 * 
 */
void dpadMotor() {
  if(Xbox.getButtonClick(UP)) {
    //BACKMOTOR.writeMicroseconds(ESCMAX - 200);
    Serial.print("UP");
  }
  else if (Xbox.getButtonClick(DOWN)) {
    //BACKMOTOR.writeMicroseconds(ESCMIN + 200);
    Serial.print("DOWN");
  }
  else {
    //BACKMOTOR.writeMicroseconds(ESCMID);
    Serial.print("NONE");
  }
  Serial.print(">");
}

/** Maps the right and left buttons of the directional pad to the servo
 *    The back servo does not require as much precision as the side motors. There are
 *    only three settings for the servo: rightmost, leftmost, and equilibrium. Furthermore,
 *    the servo will remain in its current setting unless it is told to change by a
 *    different brutton. To return to equilibrium, the 'X' button on the controller must be
 *    pressed.
 */
void dpadServo() {
  int setting = 0;
  if(Xbox.getButtonClick(RIGHT))        setting = 1; 
  else if (Xbox.getButtonClick(LEFT))   setting = 2; 
  else if (Xbox.getButtonClick(X))      setting = 0;

  if(setting == 1) {
    BACKSERVO.writeMicroseconds(WPMAX);
    Serial.print("RIGHT");
  }
  else if (setting == 2) {
    BACKSERVO.writeMicroseconds(WPMIN);
    Serial.print("LEFT");
  }
  else if (setting == 0) {
    BACKSERVO.writeMicroseconds(WPMID);
    Serial.print("NONE");
  }

  Serial.print(">");
}


/** Places timestamps for all power-mapping values from the controller >> 
 *    For every power-mapping value that the serial monitor outputs
 *    the following code generates time stamps to mark the time at which each power
 *    value is used and displays it alongside each power value. This data will later be used in 
 *    conjunction with the time values to allow the driver to visualize how much power they are using and 
 *    increase their efficiency based on analyzing these values.
 */

void digitalClockDisplay(){
  // digital clock display of the time printed to the serial monitor
  Serial.print("Time ");
  Serial.print(hour());
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
}

/** Formats the time outputted by the serial monitor in a comprehensive way >>
 *  This function puts the inputted time in the correct format and allows the 
 *  serial monitor to process the pc time in a constant templated format.
 */


void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0 in front of the hours
  
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/**Syncs the time on an electronic device to the time on the serial monitor >>
 * This message syncs the time on the PC or other monitor device with time on it to
 * the serial monitor in order to allow the serial monitor to output the time alongside
 * each mapped power level, making it easier to store data aboyt the relationship between time
 * and power level.
 */

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2019

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2019)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}//might add time parser back



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
