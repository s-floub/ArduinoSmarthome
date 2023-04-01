#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include "transmission.h"
#endif

Message createMessage(deviceType sensor, messageType messageType, int data){
  //Creates and returns Message with given specifications

  Message toReturn;

  toReturn.productWhat = PRODUCTWHAT;

  toReturn.productNum[0] = PRODUCTNUM[0];
  toReturn.productNum[1] = PRODUCTNUM[1];
  toReturn.productNum[2] = '\0';
  
  toReturn.sensor = sensor;

  toReturn.messageType = messageType;
  toReturn.data.type = intType;
  toReturn.data.data.intData = data;

  return toReturn;
}

Message createMessage(deviceType sensor, messageType messageType, char data[]){
  //Creates and returns Message with given specifications

  Message toReturn;

  toReturn.productWhat = PRODUCTWHAT;

  toReturn.productNum[0] = PRODUCTNUM[0];
  toReturn.productNum[1] = PRODUCTNUM[1];
  toReturn.productNum[2] = '\0';
  
  toReturn.sensor = sensor;

  toReturn.messageType = messageType;
  toReturn.data.type = strType; 
  strcpy(toReturn.data.data.strData, data); //I'm not checking to make sure the string is not to long, this is bad, I know

  return toReturn;
}

Message createMessage(deviceType sensor, messageType messageType, float data){
  //Creates and returns Message with given specifications

  Message toReturn;

  toReturn.productWhat = PRODUCTWHAT;

  toReturn.productNum[0] = PRODUCTNUM[0];
  toReturn.productNum[1] = PRODUCTNUM[1];
  toReturn.productNum[2] = '\0';
  
  toReturn.sensor = sensor;

  toReturn.messageType = messageType;
  toReturn.data.type = floatType;
  toReturn.data.data.floatData = data;

  return toReturn;
}

Message createMessage(deviceType sensor, messageType messageType, Request data){
  //Creates and returns Message with given specifications
  //Takes in Request and converts it to correct c string format before assembling Message

    char requestToStr[30];

    requestToStr[0] = data.type;

    requestToStr[1] = (char) data.destination[0];
    requestToStr[2] = (char) data.destination[1];
    requestToStr[3] = (char) data.destination[2];

    requestToStr[4] = data.device;

    requestToStr[5] = '\0';

    if(data.additional){
        char additionalBuffer[24];
        itoa(data.additional, additionalBuffer, 10);
        strcat(requestToStr, additionalBuffer);
    }


  return createMessage(sensor, messageType, requestToStr);
}

void sendMessage(Message message){
  //Send Message using HC12
  //Currently only sends on default channel

    HC12.write(message.productWhat);
    HC12.write(message.productNum);
    HC12.write(message.sensor);
    HC12.write(message.messageType);

    HC12.write(message.data.type);

    //Switch case prints data in the correct format
    switch(message.data.type){
        case intType:
            HC12.print(message.data.data.intData);
        break;

        case floatType:
            HC12.print(message.data.data.floatData);
        break;

        case strType:
            HC12.print(message.data.data.strData);
        break;

        default:
            //Error case.
        break;
    }

    HC12.write(message.endChar);

}

Message reciveTransmission(){ //Only call if enough avalable chars ie HC12.avalible() > 8
  //Recives transmission from HC12 and parces it into a Message

  Message toReturn;

  toReturn.productWhat = (productType) HC12.read();

  toReturn.productNum[0] = HC12.read();
  toReturn.productNum[1] = HC12.read();
  toReturn.productNum[2] = '\0';

  toReturn.sensor = (deviceType) HC12.read();
  toReturn.messageType = (messageType) HC12.read();

  toReturn.data.type = (dataType) HC12.read();

  String inputBuff = "";

  while(HC12.available() && HC12.peek() != '\n'){
    inputBuff += (char) HC12.read();
  }

    switch(toReturn.data.type){
        case intType:
            toReturn.data.data.intData = atoi(inputBuff.c_str());
        break;

        case floatType:
            toReturn.data.data.floatData = atof(inputBuff.c_str());
        break;

        case strType:
            strcpy(toReturn.data.data.strData, inputBuff.c_str());
        break;

        default:
            //Error case.
        break;
    }

  HC12.read(); //Read here to remove the newline charachter from the buffer

  return toReturn;

}

Request parseRequest(Message message){

  Request toReturn;

  toReturn.type = (messageType) message.data.data.strData[0];

  toReturn.destination[0] = message.data.data.strData[1];
  toReturn.destination[1] = message.data.data.strData[2];
  toReturn.destination[2] = message.data.data.strData[3];
  toReturn.destination[3] = '\0';

  toReturn.device = (deviceType) message.data.data.strData[4];

  return toReturn;
}