#define fori(i, n) for(int i = 0; i < n; ++i)
#define IN(i, l, r) (l < i && i < r)
#define LINR(i, l, r) (l <= i && i <= r)
#define elif else if
#include <Servo.h>
#include <Wire.h>

//User configuration:
int percent = 0;  //between -100 and 100, indicates how fast the motor 
      //will be moving when the arduino boots
int pins[] = {9, 10, 11, 3, 5, 6, 100}; //the signal output pins (as many as you'd like)
int servopin = 9;
int percentages[] = {50, 50, 50, 50, 50, 50, 50};


const int arraySize = sizeof(pins)/sizeof(byte);
Servo controllers[arraySize];
Servo cservo;
float loopIndex = 1500;
int loopStage = 1;

void setup() {
  initializeProgram();
  attachPins();
  delay(1000);
  Serial.println("INPUT PERCENTAGE");
  cservo.writeMicroseconds(1500);
  pinOtherArduino();
}


void loop() {
  writeMicrocontrollers();
  if (Serial.available() > 1) {
    int controlValue = Serial.read() - '0';
    int inputValue = Serial.parseInt();
    int blank = Serial.read();
    Serial.print("RECEIVED INSTRUCTIONS FOR ");
    Serial.println(controlValue);
    Serial.println(inputValue);
    
    if (inputValue == 50) {
      Serial.println("STOPPING");
      percentages[controlValue] = 50;
    } 
    elif (LINR(inputValue, 0, 100)) {
      setControlValues(controlValue, inputValue);
    }
    else Serial.println("INVALID VALUE");
    if(LINR(inputValue, 0, 100) && controlValue == 6) {
      writeWires();
    }
  }
}

void initializeProgram() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("PROGRAM INITIALIZED");
  Serial.println(arraySize);
}

void attachPins() {
  fori(i, arraySize) {
    Serial.print("attached ");
    Serial.print(i);
    Serial.print(" to ");
    Serial.println(pins[i]);
    controllers.attach(pins[i]);
  }
  cservo.attach(servopin);
}

void writeMicrocontrollers() {
  fori(i, arraySize) {
    if(pins[i] < 7) controllers.writeMicroseconds(percentages[i] * 14 + 800); 
    else controllers[i].writeMicroseconds(percentages[i] * 10 + 1000);
  }
}

void pinOtherArduino() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void setControlValues(int cv, int iv) {
  percentages[cv] = iv;
  Serial.print("SET VALUE TO: ");
  Serial.print(percentages[cv]);
  Serial.println("%");
}

void writeWires() {
  Wire.beginTransmission(8);
  Wire.write((byte)floor((percentages[6] * 10 + 1000) / 64));
  Wire.write((percentages[6] * 10 + 1000) - ((byte) floor((percentages[6] * 10 + 1000)/64)*64));
  Wire.endTransmission();
}
