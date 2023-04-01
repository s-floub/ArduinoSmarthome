#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "boardLogic.h"
#endif

extern SoftwareSerial HC12;

void dealWithMessage(Message message){

  if(DEBUG){
    HC12.print("message.messageType");
    HC12.println(message.messageType);
    HC12.print("message.productNum");
    HC12.println(message.productNum);
    HC12.print("message.productWhat");
    HC12.println(message.productWhat);
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
