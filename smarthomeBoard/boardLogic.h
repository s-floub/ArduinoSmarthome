#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include "messageQueue.h"
#include "mainBoard.h"

//Queues message from HC12 to the queue provided
int reciveMessageToQueue(pQueue queue);

//Takes nessasary action to deal with message
//sensor, returns if matching
//main, logs data
//actuator, moves if matching
void dealWithMessage(Message message);

//Debug function to print message to Serial port
void printMessageToSerialDEBUG(Message message);

//Debug function to print request to Serial port
void printRequestToSerialDEBUG(Request request);

#endif