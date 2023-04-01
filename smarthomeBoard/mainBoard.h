#ifndef MAINBOARD_H
#define MAINBOARD_H

#include "transmission.h"
#include "config.h"
#include <Arduino.h>

typedef struct device{
    productType productWhat; //What product (main, sensor, actuator)
    char productNum[3]; //Unique product identifier
    deviceType sensor; //What device is being accessed
    struct device *pNextDevice; // a pointer pointing to the next device in the list
} Device;

typedef struct listOfDevices{
    Device* head;
    Device* tail;
} List;

Device* createDeviceNode (productType productWhat, char productNum[3], deviceType sensor);
int insertNode (List ourList, productType productWhat, char productNum[3], deviceType sensor);
int deleteDevice (List ourList, productType productWhat, char productNum[3], deviceType sensor);
int compareDevices(Device* first, Device* second);
int queryList(List list);

#endif