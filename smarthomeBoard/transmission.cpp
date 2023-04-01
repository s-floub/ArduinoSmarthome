#include "transmission.h"

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

  if(strlen(data) > MAXDATASTRINGLEN - 1){ //Check if string is longer than allowable length

    if(DEBUG) Serial.println("data too long for message, truncating (CREATEMESSAGE)");

    //Create new string of correct length 
    char shortenedStr[MAXDATASTRINGLEN];
    shortenedStr[0] = '\0';
    strcpy(shortenedStr, data);
    shortenedStr[MAXDATASTRINGLEN - 1] = '\0'; 

    return createMessage(sensor, messageType, shortenedStr);
  }

  Message toReturn;

  toReturn.productWhat = PRODUCTWHAT;

  toReturn.productNum[0] = PRODUCTNUM[0];
  toReturn.productNum[1] = PRODUCTNUM[1];
  toReturn.productNum[2] = '\0';
  
  toReturn.sensor = sensor;

  toReturn.messageType = messageType;
  toReturn.data.type = strType; 
  strcpy(toReturn.data.data.strData, data);

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
        char additionalBuffer[7]; //int max length (base 10) is -32???, 6 digits
        itoa(data.additional, additionalBuffer, 10);
        strcat(requestToStr, additionalBuffer);
    }


  return createMessage(sensor, messageType, requestToStr);
}

void sendMessage(Message message){
  //Send Message using HC12 on current channel

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
            //Check if string is too long
            if(strlen(inputBuff.c_str()) > MAXDATASTRINGLEN - 1){
              
              if(DEBUG) Serial.println("data too long for message, truncating (RECIVETRANMISSION)");

              //Create new string of correct length 
              char shortenedStr[MAXDATASTRINGLEN];
              shortenedStr[0] = '\0';
              strcpy(shortenedStr, inputBuff.c_str());
              shortenedStr[MAXDATASTRINGLEN - 1] = '\0';

              strcpy(toReturn.data.data.strData, shortenedStr);
            }

            else{
              strcpy(toReturn.data.data.strData, inputBuff.c_str());
            }

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

  String inputBuff = "";

  for(int i = 5; message.data.data.strData[i] != '\0' && i < 11; i++){ //11 comes from max length of int with radix 10
    inputBuff += (char) HC12.read();
  }

  if(inputBuff != "") toReturn.additional = atoi(inputBuff.c_str());

  return toReturn;
}