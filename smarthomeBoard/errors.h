#ifndef ERRORS_H
#define ERRORS_H

#include "transmission.h"
#include <math.h>

typedef struct error{
    errorType errorCode; 
    unsigned long errorTime = 0; //time error occured, relative to arduino start time
}Error, *pError;

//lookup error code and match to string explaining error
//Print this error to Serial in python specified format
void errorLookupToSerial(Message messageWithError);

//Assemble and return error message ready to send, from error code
Message generateErrorMessage(Error error);

//Parse and return Error from Message
Error parseError(Message message);

#endif