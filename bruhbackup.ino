#include <Servo.h>

//User configuration:
int percent = 0;  //between -100 and 100, indicates how fast the motor 
      //will be moving when the arduino boots
int pins[] = {11, 9}; //the signal output pins (as many as you'd like)


const int arraySize = sizeof(pins)/sizeof(int);
Servo controllers[arraySize];

void setup() {
  Serial.begin(9600);
  Serial.println("PROGRAM INITIALIZED");
  Serial.println(arraySize);
  for (int i=0; i<arraySize; i++)
    controllers[i].attach(pins[i]); //associate the object to a pin
  delay(1000);
  Serial.println("INPUT PERCENTAGE");
  
}


void loop() {
  int PWMvalue = percent * 5 + 1500; //scale up to 1000-2000

  for (int i=0; i<arraySize; i++)
    controllers[i].writeMicroseconds(PWMvalue);

  if (Serial.available() > 1) {
    long proposedValue = Serial.parseInt();
    if (proposedValue == 0) {
      Serial.println("STOPPING");
      percent = 0;
    } else if (proposedValue >= -100 && proposedValue <= 100) {
      percent = proposedValue;
      Serial.print("SET VALUE TO ");
      Serial.print(percent);
      Serial.println("%");
    } else {
      Serial.println("INVALID VALUE");
    }
  }

  

}
