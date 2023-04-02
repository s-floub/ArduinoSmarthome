#include "errors.h"


void errorLookupToSerial(Message messageWithError){

}

Message generateErrorMessage(Error ourError, deviceType deviceType){
     //Max possible length of unsined long and three digit error code
    char errorToString[(int) floor( log10 ( (float) pow(sizeof(unsigned long), 2 ))) + 1 + 3];
    errorToString[0] = '\0';

    //Assemble message to string
    int dig = sprintf(errorToString, "%03i", (int) ourError.errorCode);
    sprintf(errorToString + dig, "%lu", ourError.errorTime);

    if(DEBUG) Serial.println("Our Error:");
    if(DEBUG) Serial.println(errorToString);

    return createMessage(deviceType, error, errorToString);
    
}

Error parseError(Message message){

    Error toReturn;

    if(message.messageType != error || message.data.type != strType){
        if(DEBUG) Serial.println("NOT AN ERROR");
        toReturn.errorCode = theseAreNotTheErrorsYoureLookingFor;
        return toReturn;
    } 

    char errorCode[4];
    errorCode[0] = message.data.data.strData[0];
    errorCode[1] = message.data.data.strData[1];
    errorCode[2] = message.data.data.strData[2];
    errorCode[3] = '\0';

    String errorTimeBuff = "";

    for(int i = 3; message.data.data.strData[i] != '\0' && i < 13; i++){ //11 comes from max length of unsigned long
        errorTimeBuff += (char) message.data.data.strData[i];
    }

    char *dummy;

    toReturn.errorCode = (errorType) atoi(errorCode);
    toReturn.errorTime = (unsigned long) strtoul(errorTimeBuff.c_str(), &dummy, 10);

    return toReturn;
}