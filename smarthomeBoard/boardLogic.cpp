#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "boardLogic.h"
#endif

extern SoftwareSerial HC12;

void dealWithMessage(Message message){

  HC12.print("message.messageType");
  HC12.println(message.messageType);
  HC12.print("message.productNum");
  HC12.println(message.productNum);
  HC12.print("message.productWhat");
  HC12.println(message.productWhat);
  


  switch(PRODUCTWHAT){
    case sensorBoard:
      if(message.messageType == request){
        Request theRequest = parseRequest(message);
        char whoIAm[4];
        whoIAm[0] = PRODUCTWHAT;
        whoIAm[1] = PRODUCTNUM[0];
        whoIAm[2] = PRODUCTNUM[1];
        whoIAm[3] = '\0';
        if(!strcmp(theRequest.destination, whoIAm)){
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
