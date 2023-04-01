#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <string.h>
#include <stdlib.h>
#include <SoftwareSerial.h>
#include "config.h"

extern SoftwareSerial HC12;

enum productType{ //Board type used to determine the functionallity of a board
  mainBoard = '0', //The main board is used to store data and send requests to other boards
  sensorBoard = '1', //The sensor board is equipped with sensors and will return the sensor readings when requested
  actuatorBoard = '2' //The actuator board will move a servo or other actuating device when requested
};

extern const productType PRODUCTWHAT; //What product is this board, defined in smarthomeBoard.ino
extern const char PRODUCTNUM[3]; //Unique identifying number, must be unique

enum deviceType{ //Identifiers for devices, both sensors and actuators
  main = '!',

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
    messageType type; //Type of request, wanting data, or commanding actuation
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
  char strData[MAXDATASTRINGLEN];
  float floatData;
} UnionData;

typedef struct data{ //Data to be sent / is recived
    UnionData data;
    dataType type;

    //Manual assignment operator decleration
    //Nessasary due to union and strings being strings
    void operator=(const struct data& dataC)
      {
        type = dataC.type; //Type is kept same

        //int and float copys are handled traditionally
        if (type == intType){
          data.intData = dataC.data.intData; 
        }
        else if (type == floatType){
          data.floatData = dataC.data.floatData;
        }
        else if (type == strType){
          //data.strData = '\0';
          strcpy(data.strData, dataC.data.strData); //string copy handled with strcpy
        }
        else{
          data.intData = 11111; //Should never reach here
        }

      }

} Data;

typedef struct message{ //Message, recived or to be sent
    productType productWhat; //What product (main, sensor, actuator)
    char productNum[3]; //Unique product identifier
    deviceType sensor; //What device is being accessed
    messageType messageType; //What type of message is this (data, command, error)

    Data data; //The data portion of the message
    char checkbyte = '0';
    const char endChar = '\n'; //End charachter of ALL messages

    //Manual assignment of assignment operator
    //I think the reason this is required is because productNum is a string
    void operator=(const struct message& mes)
    {
      productWhat = mes.productWhat;
      productNum[0] = mes.productNum[0];     
      productNum[1] = mes.productNum[1];    
      productNum[2] = mes.productNum[2];  
      sensor = mes.sensor;
      messageType = mes.messageType;
      checkbyte = mes.checkbyte;
      data = mes.data;
    }

} Message, *pMessage;

//Checks if message is valid, returns 0 if it is, non zero to add at end if not
int checkMessageValidity(Message message);

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

//Recives transmission from HC12 and parces it into a Message
//Only call if enough avalable chars ie HC12.avalible() > 9
//Does not check for errors or invalid messages in this function
Message reciveTransmission();

//Takes Message of Request type and parses it to Request struct
Request parseRequest(Message message);

#endif