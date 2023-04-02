
#ifndef SENSORDATARETRIEVAL_H
#define SENSORDATARETRIEVAL_H
#include "transmission.h"
#include "DHT.h"
#include "config.h"

#include <Arduino.h>

int getSensorData(deviceType d);
int THERM();
int PHOTO();

#endif