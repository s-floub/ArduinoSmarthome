#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef SOFTWARESERIAL_H
#define SOFTWARESERIAL_H
#include <SoftwareSerial.h>
#endif

extern SoftwareSerial HC12;

enum productType{ //Board type used to determine the functionallity of a board
  mainBoard = '0', //The main board is used to store data and send requests to other boards
  sensorBoard = '1', //The sensor board is equipped with sensors and will return the sensor readings when requested
  actuatorBoard = '2' //The actuator board will move a servo or other actuating device when requested
};

extern const productType PRODUCTWHAT; //What product is this board, defined in smarthomeBoard.ino
extern const char PRODUCTNUM[3]; //Unique identifying number, must be unique

enum deviceType{ //Identifiers for devices, both sensors and actuators
  photo = '1', //Photoresistor, Sensor
  pot = '2', //Potentiomiter, Sensor
  temp = '3', //Tempature, Sensor
  humid = '4', //Humidity, Sensor

  servo = 's' //Servomotor, Actuator
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

typedef struct request{ //Requests sent from main board
    requestWhat type; //Type of request, wanting data, or commanding actuation
    char destination[4]; //Who this request is going to, 3 digits (4 for \0)
    deviceType device; //What device am I requesting to/from
    int additional = 0; //Used to specify additional info. ie. servo heading
} Request;

enum dataType{ //Data Type
  intType = 'i', 
  strType = 's', //c strings, not String
  floatType = 'f',
};

typedef union unionData{ //Union for data portion of the message
  int intData;
  char strData[30]; //Max of 29 chars
  float floatData;
} UnionData;

typedef struct data{ //Data to be sent / is recived
    UnionData data;
    dataType type;
} Data;

typedef struct message{ //Message, recived or to be sent
    char productWhat; //What product (main, sensor, actuator)
    char productNum[3]; //Unique product identifier
    deviceType sensor; //What device is being accessed
    messageType messageType; //What type of message is this (data, command, error)

    Data data; //The data portion of the message
    const char endChar = '\n'; //End charachter of ALL messages

} Message;

//Create and return message with given specifications, takes int data
Message createMessage(deviceType sensor, messageType messageType, int data);

//Create and return message with given specifications, takes c string data
Message createMessage(deviceType sensor, messageType messageType, char data[]);

//Create and return message with given specifications, takes float data
Message createMessage(deviceType sensor, messageType messageType, float data);

//Create and return message with given specifications, takes Request data and converts Request to c string
Message createMessage(deviceType device, messageType messageType, Request request);

//Send Message with correct format using the HC12 Transmitter
void sendMessage(Message message);

//Recive and return Message from HC12 Transmitter
Message reciveTransmission();