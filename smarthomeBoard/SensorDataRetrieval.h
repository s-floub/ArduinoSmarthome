
#ifndef SENSORDATARETRIEVAL_H
#define SENSORDATARETRIEVAL_H
#include "transmission.h"
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2     // define the digital pin the DHT is connected to
#define thermPin A0  // define the analog pin the thermistor is connected to
#define photoPin A1  // define the analog pin the photoresistor is connected to

#include <Arduino.h>

int getSensorData(deviceType d);
int THERM();
int PHOTO();

#endif