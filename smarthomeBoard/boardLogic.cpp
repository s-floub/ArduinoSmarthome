#include "boardLogic.h"

extern SoftwareSerial HC12;

int reciveMessageToQueue(pQueue queue){

  if(HC12.available() < 6 ) return RETURN_ERR;

  Message theMessage = reciveTransmission();

  if(!checkMessageValidity(theMessage)) {

    Enqueue(queue, CreateItem(theMessage));
    return RETURN_OK;

  }

  if(DEBUG) Serial.println("RECIVEDINVALIDMESSAGE");
  return RETURN_ERR;

}

void dealWithMessage(Message message){

  if(DEBUG){
    Serial.print("message.messageType");
    Serial.println(message.messageType);
    Serial.print("message.productNum");
    Serial.println(message.productNum);
    Serial.print("message.productWhat");
    Serial.println(message.productWhat);
  }

  switch(PRODUCTWHAT){
    case sensorBoard:
      if(message.messageType == request){ //Check if Message is a request
        Request theRequest = parseRequest(message); //Parse Message to find 
        
        //string to match format of Request.destination
        char whoIAm[4];
        whoIAm[0] = PRODUCTWHAT;
        whoIAm[1] = PRODUCTNUM[0];
        whoIAm[2] = PRODUCTNUM[1];
        whoIAm[3] = '\0';

        if(!strcmp(theRequest.destination, whoIAm)){ //Check if I am being requested
          sendMessage(createMessage(message.sensor, pureData, 100)); 
        }
      }
      break;

    case actuatorBoard:

      break;

    case mainBoard:
      //If Data write to log file
      //If error write to other log file
      break;

    default:

      break;
  }

}
