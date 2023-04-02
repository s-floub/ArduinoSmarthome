#include "actuateMotors.h"
Servo myservo;



void actuateMotors(int value) {
    myservo.attach(6);  // attaches the servo on pin 6 to the servo object
    myservo.write(value);
}
