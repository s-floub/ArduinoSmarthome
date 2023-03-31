#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "boardLogic.h"
#endif

#define POTPIN A0

void dealWithMessage(Message message){

  switch(PRODUCTWHAT){
    case sensorBoard:
      if(message.messageType == request && !strcmp(message.productNum, PRODUCTNUM) && message.productWhat == PRODUCTWHAT){
        sendMessage(createMessage(message.sensor, pureData, analogRead(POTPIN)));
      }
      break;

    case actuatorBoard:

      break;

    case mainBoard:
      
      break;

    default:

      break;
  }

}
