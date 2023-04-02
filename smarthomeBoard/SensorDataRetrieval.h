#ifndef SENSORDATARETRIEVAL_H
#define SENSORDATARETRIEVAL_H
#include "transmission.h"
#include "DHT.h"
#include "config.h"

//include to allow printing
#include <Arduino.h>

//Declarations for functions 
int getSensorData(deviceType data);
int THERM();
int PHOTO();

#endif