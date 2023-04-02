#ifndef CHANNELS_H
#define CHANNELS_H

#include <SoftwareSerial.h>
#include "config.h"
#include <Arduino.h>
#include <stdio.h>

extern SoftwareSerial HC12;

//Change channel to specified Channel
//Possible channels 001-100
//Function adapted from www.HowToMechatronics.com
int changeChannel(int desiredChannel);

#endif