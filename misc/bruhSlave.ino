#include <Wire.h>
#include <Servo.h>

Servo cservo;
int servoSpeed = 1500;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(2, OUTPUT);
  pinMode(5, INPUT);
  cservo.attach(9);
  cservo.writeMicroseconds(1500);
}

void loop() {
  int v = digitalRead(5);
  //Serial.println(v);
  digitalWrite(2, v);
  delay(0.1);
  cservo.writeMicroseconds(servoSpeed);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  int index = howMany;
  int val = 0;
  while (0 < Wire.available()) { // loop through all but the last
    byte x = Wire.read(); // receive byte as a char
    Serial.println(x);         // print the char
    val += x * pow(64, index-1);
    index --;
    Serial.println(val);
    servoSpeed = val + 1;
  }
}
