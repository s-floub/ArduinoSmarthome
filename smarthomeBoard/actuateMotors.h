#ifndef ACTUATEMOTORS_H
#define ACTUATEMOTORS_H
#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include <Servo.h>

void doSetup();
void actuateMotors(int value);

#endif
