#include "actuateMotors.h"
Servo myservo;  //create servo object my servo

//Turns the motor to a specific position
void actuateMotors(int value) {
//attaches the motor at the servo pin
    myservo.attach(servoPin);  
//Writes the servos position to be the passed angle
    myservo.write(value);
}

void turnOnLEDs(){  //turns on LEDs
  pinMode(ledPIN, OUTPUT);  //Set the pin
  digitalWrite(ledPIN, HIGH);//Set the pin to HIGH, to turn it on
}

void turnOffLEDs(){ //turns off LEDs
  pinMode(ledPIN, OUTPUT);
  digitalWrite(ledPIN, LOW); //Set the pin to LOW, to turn it off

}
