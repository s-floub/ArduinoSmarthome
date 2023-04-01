#ifndef BOARDLOGIC_H
#define BOARDLOGIC_H
#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include "messageQueue.h"

int reciveMessageToQueue(pQueue queue);

void dealWithMessage(Message message);

void printMessageToSerialDEBUG(Message message);

void printRequestToSerialDEBUG(Request request);

#endif