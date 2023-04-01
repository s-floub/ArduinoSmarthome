
#ifndef SENSORDATARETRIEVAL_H
#define SENSORDATARETRIEVAL_H
#include "transmission.h"
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2  // define the digital pin the DHT is connected to
#include <Arduino.h>

int getSensorDataa(deviceType d);

#endif