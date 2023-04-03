#include "actuateMotors.h"
Servo myservo;

void actuateMotors(int value) {
    myservo.attach(servoPin);  
    myservo.write(value);
}

void turnOnLEDs(){
  pinMode(ledPIN, OUTPUT);
  digitalWrite(ledPIN, HIGH);   
}
