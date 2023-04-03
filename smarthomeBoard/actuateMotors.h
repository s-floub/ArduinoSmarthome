#ifndef ACTUATEMOTORS_H
#define ACTUATEMOTORS_H
#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include <Servo.h>

//Function to just move the motor based on a value between 0 and 180 degrees 
void actuateMotors(int value);
void turnOnLEDs();  //Function to turn on LED
void turnOffLEDs(); //Function to turn off LED
#endif
