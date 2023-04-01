#ifndef CHANNELS_H
#define CHANNELS_H

#include <SoftwareSerial.h>
#include "config.h"
#include <Arduino.h>
#include <stdio.h>

extern SoftwareSerial HC12;

int changeChannel(int desiredChannel);

#endif