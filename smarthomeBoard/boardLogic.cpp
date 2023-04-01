#include "boardLogic.h"
#include "SensorDataRetrieval.h"


extern SoftwareSerial HC12;

int reciveMessageToQueue(pQueue queue){

  if(HC12.available() < 6 ) return RETURN_ERR;

  Message theMessage = reciveTransmission();

  if(!checkMessageValidity(theMessage)) {

    Enqueue(queue, CreateItem(theMessage));
    return RETURN_OK;

  }

  if(DEBUG) Serial.println("RECIVEDINVALIDMESSAGE");
  if(DEBUG) Serial.println(checkMessageValidity(theMessage));
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

void printMessageToSerialDEBUG(Message message){
  if (!DEBUG) return;

    Serial.println("message.productWhat");
    Serial.write(message.productWhat);
     Serial.println("message.productNum");
    Serial.write(message.productNum);
     Serial.println("message.sensor");
    Serial.write(message.sensor);
     Serial.println("message.messageType");
    Serial.write(message.messageType);
     Serial.println("message.checkbyte");
    Serial.write(message.checkbyte);

     Serial.println("message.data.type");
    Serial.write(message.data.type);

    //Switch case prints data in the correct format
    switch(message.data.type){
        case intType:
         Serial.println("message.data.data.intData");
            Serial.print(message.data.data.intData);
        break;

        case floatType:
         Serial.println("message.data.data.floatData");
            Serial.print(message.data.data.floatData);
        break;

        case strType:
         Serial.println("message.data.data.strData");
            Serial.print(message.data.data.strData);
        break;

        default:
            //Error case.
        break;
    }
 Serial.println("message.endChar");
    Serial.write(message.endChar);
}
