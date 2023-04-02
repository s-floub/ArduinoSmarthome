#ifndef MAINBOARD_H
#define MAINBOARD_H

#include "transmission.h"
#include "config.h"
#include <Arduino.h>
#include "boardLogic.h"

typedef struct device{
    productType productWhat; //What product (main, sensor, actuator)
    char productNum[3]; //Unique product identifier
    deviceType sensor; //What device is being accessed
    struct device *pNextDevice; // a pointer pointing to the next device in the list
} Device;

typedef struct listOfDevices{
    Device* head = NULL;
    Device* tail = NULL;
} List, *pList;

// Initialise and return empty List pointer
pList initialiseList();

// Create a new Device node
Device* createDeviceNode (productType productWhat, char productNum[3], deviceType sensor);

// Inserts Device node at end of list
int insertNode (pList ourList, productType productWhat, char productNum[3], deviceType sensor);

//NEEDS TESTING
// Finds Device that matches values and remove that node from the list
int deleteDevice (pList ourList, productType productWhat, char productNum[3], deviceType sensor);

//Compares two Device pointers, returns 0 if same
int compareDevices(Device* first, Device* second);

//Sends request to device, waits for return, and parses return with dealWithMessage()
int queryDevice(Device* pDevice, pQueue messageQueue);

//query all Devices in list
int queryList(pList list, pQueue messageQueue);

//Outputs message data in format to display on python based graphs and store in csv
int outputToSerialInPythonFormat(Message message);

#endif