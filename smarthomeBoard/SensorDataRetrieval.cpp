#include "SensorDataRetrieval.h"

int getSensorData(deviceType d) {
  int roomTemperature;
  int humidity;
  int photoresistor;
  
  DHT dht(DHTPIN, DHTTYPE);

  //Send request was for TEMP
  if(d==temp) 
  {
  roomTemperature = dht.readTemperature();
  if(isnan(humidity) || isnan(roomTemperature)){
    //DHT not connected, so instead just grab a temp from thermistor
    roomTemperature = THERM();
  }

    Serial.println(roomTemperature);
    return roomTemperature;
  }
  //Send request was for HUMIDITY
  if(d==humid) 
  {
    humidity = dht.readHumidity();
    return humidity;
  }
//Send request was for PHOTORESISTANCE
if(d==photo)
{
  photoresistor = PHOTO();
  return photoresistor;

}
}

//CALCULATION FUNCTIONS
int THERM(){  //Function to calculate thermistor temperature
  int Vo;
  float R1 = 10000;
  float logR2, R2, tKelvin, tCelsius;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  Vo = analogRead(thermPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0); 
  logR2 = log(R2);
  tKelvin = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  tCelsius = tKelvin - 273.15;
  return tCelsius;
}

int PHOTO(){  //Function to calculate photoresistance in lux
int resistance = 10000;
int readDelay = 1000;
int reading = analogRead(photoPin);
float voltage = reading * 5.0 / 1024.0;
float Rphoto = 5.0 / voltage * resistance - resistance;
float lightIntensity = 500/(Rphoto/1000);
delay(readDelay);
return lightIntensity;
}

