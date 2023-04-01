#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif

#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include "transmission.h"
#endif

#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "boardLogic.h"
#endif

#ifndef MAINBOARD_H
#define MAINBOARD_H
#include "mainBoard.h"
#endif

#define POTPIN A0

// 0: mother, 1: sensor, 2: actuator
const productType PRODUCTWHAT = sensorBoard;

// No meaning besides as identifier
const char PRODUCTNUM[3] = "11";

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

  pinMode(POTPIN, INPUT);
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
