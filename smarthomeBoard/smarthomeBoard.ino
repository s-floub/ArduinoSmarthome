#include "config.h"
#include "transmission.h"
#include "boardLogic.h"
#include "mainBoard.h"
#include "channels.h"
#include "messageQueue.h"
#include "actuateMotors.h"

// 0: mother, 1: sensor, 2: actuator
productType PRODUCTWHAT;

// No meaning besides as identifier
char PRODUCTNUM[3];

SoftwareSerial HC12(HC12TXPIN, HC12RXPIN);

pQueue messageQueue = CreateQueue();
pQueue errorQueue = CreateQueue();

pList devicesList = initialiseList();

int halfHeight;

void setup() {
  Serial.begin(9600);  // Serial port to computer
  HC12.begin(9600);    // Serial port to HC12

  pinMode(POTPIN, INPUT);
  pinMode(HC12SETPIN, OUTPUT);

  digitalWrite(HC12SETPIN, HIGH);

  initProduct();

  changeChannel(DEFAULTCHANNEL);

  if (PRODUCTWHAT == mainBoard){
  insertNode(devicesList, sensorBoard, "57", temp);
  insertNode(devicesList, sensorBoard, "57", pot);
  insertNode(devicesList, sensorBoard, "57", photo);
  insertNode(devicesList, sensorBoard, "11", humid);
  insertNode(devicesList, sensorBoard, "11", temp);
  insertNode(devicesList, sensorBoard, "11", pot);
  halfHeight = 3;
  }
}

char readBuffer[100];
int adjust = 0;

int runOnce = 0;

void loop() {

  if(PRINTEVERYTHING && DEBUG){
  while (HC12.available()) {        // If HC-12 has data
      Serial.write(HC12.read());
    };  // Send the data to Serial
  }

  else if (PRODUCTWHAT == mainBoard) {

      if(DEBUG) Serial.println(F("sizeof Device"));
      if(DEBUG) Serial.println((int) sizeof (Device));
      if(DEBUG) Serial.println(F("sizeof queueItem"));
      if(DEBUG) Serial.println((int) sizeof (QueueItem));
      if(DEBUG) Serial.println(F("sizeof Queue"));
      if(DEBUG) Serial.println((int) sizeof (Queue));
      if(DEBUG) Serial.println(F("sizeof LL"));
      if(DEBUG) Serial.println((int) sizeof (List));

    queryList(devicesList, messageQueue, 0);

    delay(2000);

    queryList(devicesList, messageQueue, halfHeight);

    delay(2000);

  }

  else {
    delay(1000);

    if (HC12.available() >= MINMESSAGELEN) {
      delay(1000);
      reciveMessageToQueue(messageQueue);
    }

    if (messageQueue->count > 0) {
      Message theMessage;
      Dequeue(messageQueue, theMessage);
      dealWithMessage(theMessage);
    }
  }
}
