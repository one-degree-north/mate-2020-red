/**
 * TODO: Map XBOX triggers to motors (triggers 0-1023)
 * TODO: Map XBOX joysticks to servos
 * TODO: Map horizontal movement of joysticks to act as modifier for strength of motors
 */

#include <XBOXONE.h>
#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXONE Xbox(&Usb);

//Revision 1.3 (DEV-09947)
#define MAX_RESET 7 //MAX3421E pin 12
#define MAX_GPX   8 //MAX3421E pin 17

#define ESCPIN 6
#define WPPIN 5

#define TRIGGERMAX 1023
#define TRIGGERMIN 0
#define ESCMIN 1000
#define ESCMAX 2000
#define JOYSTICKMAX 32767
#define JOYSTICKMIN -32768
#define WPMAX 0 // TODO: update
#define WPMIN 0 // TODO: update

Servo ESC;
Servo WP;

void setup() { 
  attachAndPin();
  serialConnect();
  setPwmFrequency(ESCPIN, 64); // force pwm at 500Hz
  setPwmFrequency(WPPIN, 64);
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
  ESC.attach(ESCPIN);
  WP.attach(WPPIN);
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
  ESC.attach(ESCPIN,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
  delay(200); // wait
  ESC.writeMicroseconds(1800); // throttle init
  delay(200); // wait
  ESC.writeMicroseconds(1500); // stop
  Serial.println("ESC init done");
  WP.attach(WPPIN,900,2100);
  WP.writeMicroseconds(1500);
  delay(200);
  WP.writeMicroseconds(1500);
}

void leftJoystick() {
  // TODO: When the left servo arrives
}

void rightJoystick() {
  if (Xbox.getAnalogHat(RightHatY) > 4000 || Xbox.getAnalogHat(RightHatY) < -4000) {
    Serial.print(F("RightHatY: "));
    Serial.print(Xbox.getAnalogHat(RightHatY));
    double y = (double) ((double) 1160 / (double) 65534);
    double b = (double) (Xbox.getAnalogHat(RightHatY)) * y;
    b += 1520;
    Serial.print(", output: ");
    Serial.print(b);
    WP.writeMicroseconds(b);
    } else {
      Serial.print("no RightHatY, resetting to 1520   ");
      WP.writeMicroseconds(1520);
  }
}

void rightTrigger() {
  if(Xbox.getButtonPress(R2) > 20) {
    double inp = Xbox.getButtonPress(R2);
    double ret = map(inp, TRIGGERMIN, TRIGGERMAX, ESCMIN, ESCMAX);
    Serial.print(", output: ");
    Serial.print(ret);
    ESC.writeMicroseconds(ret);
    // TODO: Bumper to make value negative
  }
  else {
    Serial.print("no R2 Trigger, resetting to 1500");
    ESC.writeMicroseconds(1500);
  }
  Serial.println();
}

void leftTrigger() {
  if(Xbox.getButtonPress(L2) > 20) {
    // TODO: When the left motor arrives
  }
}
