// MACROS
#define fori(i, n) for(int i = 0; i < n; ++i)                                         // for loop
#define foris(i, a, n) for(int i = a; i < n; ++i)                                     // for loop, specific
#define IN(i, l, r) (l < i && i < r)                                                  // check if the value is within the given range (exclusive)
#define LINR(i, l, r) (l <= i && i <= r)                                              // check if the value is within the given range (inclusive)
#define elif else if                                                                  // else if but in python

// IMPORTS
#include <Servo.h>
#include <Wire.h>

// USER CONFIG:
int percent = 0;                                                                      // between -100 and 100, indicates how fast the motor 
                                                                                      // will be moving when the arduino boots
int pins[] = {9, 10, 11, 3, 5, 6, 100};                                               // the signal output pins (as many as you'd like)
int servopin = 9;                                                                     // the pin of the servo
int percentages[] = {50, 50, 50, 50, 50, 50, 50};                                     // percentages for each pin (which are the same)
                                                                                      // each pin is paired to a percentage


const int arraySize = sizeof(pins)/sizeof(byte);                                      // the amount of pins
Servo controllers[arraySize];                                                         // create an array of servos
const int pin = 6;
Servo ESC;

void setup() {
  initializeProgram();                                                                // initializes program -- prints
  attachPins();                                                                       // attaches all the pins
  setPwmFrequency(pin, 64); // force pwm at 500Hz
  ESC.attach(pin,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  delay(200); // wait
  ESC.writeMicroseconds(1800); // throttle init
  delay(200); // wait
  ESC.writeMicroseconds(1500); // stop
  delay(1000);                                                                        // delay for 1 second
  Serial.println("INPUT PERCENTAGE");                                                 // request user input without receiving user input yet
  pinOtherArduino();                                                                  // pins the slave arduino
}


void loop() {
  writeMicrocontrollers();                                                            // write all the microcontrollers with their given values
  if (Serial.available() > 1) {                                                       // if the console is available to receive input
    int controlValue = Serial.read() - '0';                                           // read
    int inputValue = Serial.parseInt();                                               // get the input value for the strength of motors
    int blank = Serial.read();
    Serial.print("RECEIVED INSTRUCTIONS FOR ");                                       // print the values given
    Serial.println(controlValue);
    Serial.println(inputValue);
    
    if (inputValue == 50) {                                                           // if the input value is 50%, stop
      Serial.println("STOPPING");
      percentages[controlValue] = 50;
    } 
    elif (LINR(inputValue, 0, 100)) {                                                 // else if the input value is within 0 and 100 (inclusive), write the control values
      setControlValues(controlValue, inputValue);
    }
    else Serial.println("INVALID VALUE");                                             // else then the value is not within 0 and 100 and is therefore invalid, tell the user
    if(LINR(inputValue, 0, 100) && controlValue == 6) {                               // write the wires
      writeWires();
    }
  }
}

void initializeProgram() {                                                            // initializes the wire and serials, tells the user
  Wire.begin();
  Serial.begin(9600);
  Serial.println("PROGRAM INITIALIZED");
  Serial.println(arraySize);
}

void attachPins() {                                                                   // attaches all the pins
  fori(i, arraySize) {
    Serial.print("attached ");
    Serial.print(i);
    Serial.print(" to ");
    Serial.println(pins[i]);
    controllers.attach(pins[i]);
  }
  cservo.attach(servopin);
}

void writeMicrocontrollers() {                                                        // write all the controller's seconds 
  fori(i, arraySize) {
    if(pins[i] < 7) controllers.writeMicroseconds(percentages[i] * 14 + 800); 
    else controllers[i].writeMicroseconds(percentages[i] * 10 + 1000);
  }
}

void pinOtherArduino() {                                                              // pins the slave arduino
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void setControlValues(int cv, int iv) {                                               // sets the percentage values for the controllers
  percentages[cv] = iv;
  Serial.print("SET VALUE TO: ");
  Serial.print(percentages[cv]);
  Serial.println("%");
}

void writeWires() {                                                                   // write the wires
  Wire.beginTransmission(8);
  Wire.write((byte)floor((percentages[6] * 10 + 1000) / 64));
  Wire.write((percentages[6] * 10 + 1000) - ((byte) floor((percentages[6] * 10 + 1000)/64)*64));
  Wire.endTransmission();
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
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
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
