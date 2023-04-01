#include "SensorDataRetrieval.h"

//**NOTE**:
/* This function works for simply recieving temperature in C and humidity %
(This is the only sensor attatched to this arduino) */

int getSensorDataa(deviceType d) {
  DHT dht(DHTPIN, DHTTYPE);

  //Sent value was for TEMP
  if(d==temp) {
    int roomTemperature = dht.readTemperature();
    Serial.println(roomTemperature);
    return roomTemperature;
  }
  //Sent value was for HUMIDITY
  if(d==humid) {
    int humidity = dht.readHumidity();
    return humidity;
  }
}

