#include "channels.h"


int changeChannel(int desiredChannel){

    if (desiredChannel > 100 || desiredChannel < 1) return RETURN_ERR; //Check if valid Channel

    //Empty avalible chars in HC12, if not done will cause errors 
    while(HC12.available()){
      HC12.read();
    }

    //convert channel int to string with leading zeros
    char channel[4];
    sprintf(channel, "%03i\0", desiredChannel);

    char toSend[8];
    strcpy(toSend, "AT+C\0");
    strcat(toSend, channel);

    digitalWrite(HC12SETPIN, LOW);       // Set HC-12 into AT Command mode
    delay(200);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(toSend);                  // Send AT Command to HC-12
    delay(200);

    String inputBuffer = "";
    
    while (HC12.available()) {                   // If HC-12 has data (the AT Command response)
      inputBuffer += (char) HC12.read();         //Add incoming chars to inputBuffer
    }

    if(DEBUG) Serial.print("Channel changed, "); 
    if(DEBUG) Serial.println(inputBuffer);
    digitalWrite(HC12SETPIN, HIGH);               // Exit AT Command mode

    //Verify that channel was changed correctly
    char toCheck[8];
    toCheck[0] = '\0';
    strcpy(toCheck, toSend);
    toCheck[0] = 'O';
    toCheck[1] = 'K';

    if(strcmp(inputBuffer.c_str(), toCheck)) return RETURN_ERR;
    return RETURN_OK;
}
