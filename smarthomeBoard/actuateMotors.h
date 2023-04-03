#ifndef ACTUATEMOTORS_H
#define ACTUATEMOTORS_H
#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include <Servo.h>

void actuateMotors(int value);
void turnOnLEDs();
#endif