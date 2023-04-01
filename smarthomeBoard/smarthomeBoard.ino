#include "transmission.h"
#include "config.h"
#include "boardLogic.h"
#include "mainBoard.h"
#include "channels.h"

// 0: mother, 1: sensor, 2: actuator
const productType PRODUCTWHAT = sensorBoard;

// No meaning besides as identifier
const char PRODUCTNUM[3] = "11";

SoftwareSerial HC12(HC12TXPIN, HC12RXPIN);

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

  pinMode(POTPIN, INPUT);
  pinMode(HC12SETPIN, OUTPUT);

  digitalWrite(HC12SETPIN, HIGH);

  changeChannel(13);
}

char readBuffer[100];

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

  Request toSend;

  toSend.destination[0] = '1';
  toSend.destination[1] = '1';
  toSend.destination[2] = '1';

  toSend.device = temp;

  toSend.type = request;

  toSend.additional = 0;
  
   sendMessage(createMessage(main, request, toSend));

   delay(5000);

}

else{
   if(HC12.available() > 8){
   delay(1000);
   dealWithMessage(reciveTransmission());
 }
}
    
  }
