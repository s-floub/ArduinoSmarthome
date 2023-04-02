#include "SensorDataRetrieval.h"

//Function to handle data retrieval from sensors
int getSensorData(deviceType data) {

  //Variables to return specific sensor readings
  /**NOTE: They are initialized as floats but returned 
  as integers in each case, so all math can be done with most significant digits **/
  float roomTemperature, humidity, photoresistor;
  int potentiometer;  //Potentiometer is an integer as it is always a 0-1023 reading

  //Initialize DHT
  //This is apart of the library for reading temperature and humidity
  DHT dht(DHTPIN, DHTTYPE);



  /**Send request was for TEMPERATURE**/
  if (data == temp) {
    roomTemperature = dht.readTemperature();  //Get sensor reading for temperature from DHT module

    //Case for if this cluster does not contain the DHT sensor, but rather a thermistor
    //If it is the thermistor, NaN will be returned for room temperature
    if (isnan(roomTemperature)) {

      //In this case, just return the temperature calculated by the thermistor function
      roomTemperature = THERM();
    }
    //Print the temperature for debugging purposes
    Serial.print("Temperature: ");
    Serial.print((int)roomTemperature);
    Serial.println(" C");

    //Send TEMPERATURE
    return (int)roomTemperature;
  }




  /**Send request was for HUMIDITY**/
  if (data == humid) {
    humidity = dht.readHumidity();  //Get sensor reading for humidity from DHT module

    /*Case for if this cluster does not contain the DHT sensor, and NaN will be returned for humidity
    Since their is no alternative to obtain humidity, either an error code or nothing must be returned */
    if (isnan(humidity)) {
      return;
    }
    //Print the humidity for debugging purposes
    Serial.print("Humidity: ");
    Serial.print((int)humidity);
    Serial.print("%");

    //Send HUMIDITY
    return (int)humidity;
  }



  /**Send request was for PHOTORESISTANCE**/
  if (data == photo) {
    photoresistor = PHOTO();  //Get sensor reading for photoresistance

    /*Case for if this cluster does not contain any photoresistor, and NaN will be returned for photo
  Since their is no alternative to obtain photo, either an error code or nothing must be returned */
    if (isnan(photoresistor)) {
      return;  //error code
    }
    //Print the humidity for debugging purposes
    Serial.print("Lux: ");
    Serial.print((int)photoresistor);
    Serial.print(" lx");

    //Send PHOTORESISTANCE
    return (int)photoresistor;
  }



  /**Send request was for POTENTIOMETER**/
  if (data == pot) {
    //Get sensor reading for photoresistance
    // No math is done as it is just being returned as an analog reading
    potentiometer = analogRead(POTPIN);
    //Just a check to ensure readings are coming through
    if (isnan(POTPIN)) {
      return;
    }
    //Send POTENTIOMETER
    return potentiometer;
  }
}

/***CALCULATION FUNCTIONS***/
/*These are all functions that contain any strenous math required for calculating 
light intensity from the photoresistor and temperature from the thermistor*/


int THERM() {  //Function to calculate thermistor temperature in celsius

  //Math from: https://www.circuitgeeks.com/arduino-temperature-sensor-thermistor/

  int Vo;                                                    //Gets intital voltage reading from the thermistor pin
  float resistance = 10000;                                  //Sets the resistance as 10k Ohm
  float lognewResistance, newResistance, tKelvin, tCelsius;  //Variables for values to be calculated

  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;  //Constants for calculations

  Vo = analogRead(thermPin);  //Grabs voltage from thermistor pin

  newResistance = resistance * (1023.0 / (float)Vo - 1.0);  //Calculates new resistance

  lognewResistance = log(newResistance);  //Takes logarithm of new resistance

  //Calculates temperature in kelvin from the resistances and constants
  tKelvin = (1.0 / (c1 + c2 * lognewResistance + c3 * lognewResistance * lognewResistance * lognewResistance));

  tCelsius = tKelvin - 273.15;  //calculates celsius by subtracting the kelvin degreee from lowest kelvin
  return tCelsius;              //returns the temperature in celsius
}

int PHOTO() {  //Function to calculate photoresistance in lux

  //Math from: https://www.circuitbasics.com/how-to-use-photoresistors-to-detect-light-on-an-arduino/

  int resistance = 10000;  //Sets resistance to 10k Ohm
  int readDelay = 1000;    //Sets a delay for recieving

  int voltage = (analogRead(photoPin)) * 5.0 / 1024.0;  //Gets the voltage reading from the photoresistor

  //Math to calculate the light intensity
  float Rphoto = 5.0 / voltage * resistance - resistance;
  float lightIntensity = 500 / (Rphoto / 1000);

  delay(readDelay);       //Delay for 1 second
  return lightIntensity;  //returns the light intensity in lux
}
