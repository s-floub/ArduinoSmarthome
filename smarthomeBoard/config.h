#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "enums.h"

extern productType PRODUCTWHAT; //What product is this board, defined in config.h
extern char PRODUCTNUM[3]; //Unique identifying number, must be unique

//Function to initalise PRODUCTNUM and PRODUCTWHAT from config.h values
void initProduct();

//Constants
#define RETURN_OK 0
#define RETURN_ERR -1
#define MINMESSAGELEN 9

//Debugging defines
#define PRINTEVERYTHING 0
#define MAINBOARD 1

#define DEBUG 0

//Changeable constants
#define MESSAGETIMEOUT 3000
#define MAXDATASTRINGLEN 20 //Must be at least 14 to correctly handle error messages
#define DEFAULTCHANNEL 13
#define CHECKSUMDIG 41 //Should be a prime number less than 78

//Pin Defintions
#define POTPIN A2
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2     // define the digital pin the DHT is connected to
#define thermPin A0  // define the analog pin the thermistor is connected to
#define photoPin A1  // define the analog pin the photoresistor is connected to

#define HC12TXPIN 10 //Might need to be pwm
#define HC12RXPIN 11 //Might need to be pwm
#define HC12SETPIN 12

// 0: mother, 1: sensor, 2: actuator
const productType whatProduct = mainBoard;

// No meaning besides as identifier
const char numberProduct[3] = "00";

#endif
