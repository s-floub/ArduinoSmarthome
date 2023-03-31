/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define PRINTEVERYTHING 0

#include "transmission.h"

#define POTPIN A0

// 0: mother, 1: sensor, 2: actuator
const productType PRODUCTWHAT = mainBoard;

// No meaning besides as identifier
const char PRODUCTNUM[3] = "00";

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
else {
    sendMessage(createMessage(pot, pureData, (float) (analogRead(POTPIN) + 0.5)));

    delay(2000);
}
    
  }

void dealWithMessage(){

  switch(PRODUCTWHAT){
    case sensorBoard:
      
      break;

    case actuatorBoard:

      break;

    case mainBoard:

      break;

    default:

      break;
  }

}
