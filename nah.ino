#define fori(i, n) for(int i = 0; i < n; ++i)
#define IN(i, l, r) (l < i && i < r)
#define LINR(i, l, r) (l <= i && i <= r)
#define elif else if
#include <Servo.h>
#include <Wire.h>

//User configuration
int percent = 0;                                                                                //between -100 and 100, indicates how fast the motor 
                                                                                                //will be moving when the arduino boots
int pins[] = {11, 10};                                                                          //the signal output pins (as many as you'd like)
int servopin = 9;


const int arraySize = sizeof(pins)/sizeof(int);
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
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}


void loop() {
  if (percent > 0){
    int PWMvalue = percent * 10 + 1000;                                                          //scale up to 1000-2000
    cservo.writeMicroseconds(percent * 14 + 800);
    fori (i, arraySize) {
      controllers[i].writeMicroseconds(PWMvalue);
    }
  }
  if (Serial.available() > 1) {
    char controlValue = Serial.read();
    int inputValue = Serial.parseInt();
    
    if (inputValue == 50) {
      Serial.println("STOPPING");
      percent = 50;
    } elif (LINR(inputValue, 0, 100)) {
      percent = inputValue;
      Serial.print("SET VALUE TO ");
      Serial.print(percent);
      Serial.println("%");
    } else {
      Serial.println("INVALID VALUE");
    }
  }
}

void initializeProgram() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("PROGRAM INITIALIZED");
}

void attachPins() {
  Serial.println(arraySize);
  fori(i, arraySize) {
    controllers[i].attach(pins[i]);
  }
  cservo.attach(servopin);
}
