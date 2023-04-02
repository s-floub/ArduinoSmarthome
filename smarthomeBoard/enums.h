#ifndef ENUMS_H
#define ENUMS_H

enum productType{ //Board type used to determine the functionallity of a board
  mainBoard = '0', //The main board is used to store data and send requests to other boards
  sensorBoard = '1', //The sensor board is equipped with sensors and will return the sensor readings when requested
  actuatorBoard = '2' //The actuator board will move a servo or other actuating device when requested
};

enum deviceType{ //Identifiers for devices, both sensors and actuators
  main, exception = '!',

  photo = '1', //Photoresistor, Sensor
  pot = '2', //Potentiomiter, Sensor
  temp = '3', //Tempature, Sensor
  humid = '4', //Humidity, Sensor

  servo = 's', //Servomotor, Actuator

  errors = 'e' //Errors stored on the board, not sent immideitly to avoid message clashing
};

enum messageType{ //Identifier for type of message
  pureData = '-', //Sending data
  request = '?', //Requesting data
  error = 'e', //Error message
  command = 'c' //Commanding actuation
};

enum requestWhat{ //Request types
    intel = 'd', //Requesting Data
    action = 'c' //Commanding actuation
};

enum dataType{ //Data Type
  intType = 'i', 
  strType = 's', //c strings, not String
  floatType = 'f',
};

enum errorType{
    success = 100,
    theseAreNotTheErrorsYoureLookingFor = 123,
    invalidTransmission = 111,
    noResponce = 222,
    invalidServoValue = 333,
    deviceNotExist = 444,
    recivedTransmissionFromSelf = 555
};

#endif