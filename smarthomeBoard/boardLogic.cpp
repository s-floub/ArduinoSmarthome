#include "boardLogic.h"
#include "SensorDataRetrieval.h"
#include "actuateMotors.h"

extern SoftwareSerial HC12;

int reciveMessageToQueue(pQueue queue){

  //Check that there is enough data for a message to be possible
  if(HC12.available() < MINMESSAGELEN) return RETURN_ERR;

  //delay to allow new HC12 data to come in
  delay(100);

  Message theMessage = reciveTransmission();

  //Check if message is valid (not corrupted)
  if(!checkMessageValidity(theMessage)) {

    //If valid add to queue
    Enqueue(queue, CreateItem(theMessage));
    return RETURN_OK;
  }

  if(DEBUG) Serial.println(F("RECIVEDINVALIDMESSAGE"));
  if(DEBUG) Serial.println(checkMessageValidity(theMessage));
  return RETURN_ERR;

}

void dealWithMessage(Message message){

  if(DEBUG){
    Serial.print(F("message.messageType "));
    Serial.println(message.messageType);
    Serial.print(F("message.productNum "));
    Serial.println(message.productNum);
    Serial.print(F("message.productWhat "));
    Serial.println(message.productWhat);
    Serial.print(F("message.sensor "));
    Serial.println(message.sensor);
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

        if(DEBUG) {
          Serial.print(F("whoIAm: "));
          Serial.println(whoIAm);
          Serial.print(F("destination "));
          Serial.println(theRequest.destination);
        }

        if(!strcmp(theRequest.destination, whoIAm)){ //Check if I am being requested
          sendMessage(createMessage(theRequest.device, pureData, getSensorData(theRequest.device))); 
        }
      }
      break;

    case actuatorBoard:
    if(message.messageType == command){ //Check if Message is a request
        Request theRequest = parseRequest(message); //Parse Message to find 

        //string to match format of Request.destination
        char whoIAm[4];
        whoIAm[0] = PRODUCTWHAT;
        whoIAm[1] = PRODUCTNUM[0];
        whoIAm[2] = PRODUCTNUM[1];
        whoIAm[3] = '\0';

        if(DEBUG) {
          Serial.print(F("whoIAm: "));
          Serial.println(whoIAm);
          Serial.print(F("destination "));
          Serial.println(theRequest.destination);
        }

        if(!strcmp(theRequest.destination, whoIAm)){ 
          // actuateMotors(theRequest.device);
        }
      }

      break;

    case mainBoard:
      //If Data write to log file
      //If error write to other log file

      //printMessageToSerialDEBUG(message);
      outputToSerialInPythonFormat(message);


      break;

    default:

      break;
  }

}

void printMessageToSerialDEBUG(Message message){
  if (!DEBUG) return;

    Serial.print(F("message.productWhat "));
    Serial.println((char) message.productWhat);
     Serial.print(F("message.productNum "));
    Serial.println(message.productNum);
     Serial.print(F("message.sensor "));
    Serial.println((char) message.sensor);
     Serial.print(F("message.messageType "));
    Serial.println((char) message.messageType);
     Serial.print(F("message.checkbyte"));
    Serial.println((char) message.checkbyte);

     Serial.println(F("message.data.type"));
    Serial.write(message.data.type);

    //Switch case prints data in the correct format
    switch(message.data.type){
        case intType:
            Serial.print(F("message.data.data.intData "));
            Serial.println(message.data.data.intData);
        break;

        case floatType:
            Serial.print(F("message.data.data.floatData"));
            Serial.println(message.data.data.floatData);
        break;

        case strType:
          Serial.print(F("message.data.data.strData"));
          Serial.println(message.data.data.strData);
        break;

        default:
            //Error case.
        break;
    }
 Serial.println(F("message.endChar"));
    Serial.write(message.endChar);
}



void printRequestToSerialDEBUG(Request request){
  if (!DEBUG) return;

  Serial.print(F("request.destination"));
  Serial.println(request.destination);
   Serial.print(F("request.device"));
  Serial.println((char) request.device);
   Serial.print(F("request.type"));
  Serial.println((char) request.type);
   Serial.print(F("request.additional"));
  Serial.println((int) request.additional);

}
