#include "transmission.h"
#include "config.h"
#include "boardLogic.h"
#include "mainBoard.h"
#include "channels.h"
#include "messageQueue.h"

// 0: mother, 1: sensor, 2: actuator
const productType PRODUCTWHAT = sensorBoard;

// No meaning besides as identifier
const char PRODUCTNUM[3] = "11";

SoftwareSerial HC12(HC12TXPIN, HC12RXPIN);

pQueue messageQueue = CreateQueue();

pList devicesList = initialiseList();

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

  pinMode(POTPIN, INPUT);
  pinMode(HC12SETPIN, OUTPUT);

  digitalWrite(HC12SETPIN, HIGH);

  changeChannel(DEFAULTCHANNEL);

  if (PRODUCTWHAT == mainBoard){
  insertNode(devicesList, sensorBoard, "11", humid);
  insertNode(devicesList, sensorBoard, "11", temp);

  }

}

char readBuffer[100];
int adjust = 0;

int runOnce = 0;

void loop() {


  if(PRINTEVERYTHING){
  while (HC12.available()) {        // If HC-12 has data
    //Serial.write("got somthign\n");
   
      Serial.write(HC12.read());

      };      // Send the data to Serial 
      }

   /* while (HC12.available()) {             // If HC-12 has data
      char incomingByte = HC12.read();          // Store each icoming byte from HC-12
      strncat(readBuffer, char(incomingByte), 1);    // Add each byte to ReadBuffer string variable
    }*/
else if (MAINBOARD){

   //ProductNum = 11

  queryList(devicesList, messageQueue);
/*
  adjust++;

  Request toSend;

  toSend.destination[0] = '0' + (adjust % 8);
  toSend.destination[1] = '1';
  toSend.destination[2] = '1';

  toSend.device = temp;

  toSend.type = request;

  toSend.additional = (int) (adjust*20);
  
  Message ourMessage = createMessage(main, request, toSend);

   Serial.print("Checkbyte ");
   Serial.println(checkMessageValidity(ourMessage));
   sendMessage(ourMessage);*/

   delay(10000);

}

else{
  delay(1000);

   if(HC12.available() > MINMESSAGELEN){
   delay(1000);
   reciveMessageToQueue(messageQueue);
 }

 if(messageQueue->count > 0){
  Message theMessage;
  Dequeue(messageQueue, theMessage);
  dealWithMessage(theMessage);
 }
/*
  delay(2000);
  Serial.print("messageQueue->count ");
  Serial.println(messageQueue->count);
}

  if(messageQueue->count > 10){
    while(messageQueue->count > 0){
      Message messageToPrint;
      Dequeue(messageQueue, messageToPrint);
      printMessageToSerialDEBUG(messageToPrint);
      Serial.print("-----------");
      Request requestFromMessage = parseRequest(messageToPrint);
      printRequestToSerialDEBUG(requestFromMessage);
        Serial.print("messageQueue->count ");
  Serial.println(messageQueue->count);
      delay(7000);
      
    }*/
  }
    
  }
