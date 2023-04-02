#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//Constants
#define RETURN_OK 0
#define RETURN_ERR -1
#define MINMESSAGELEN 9

//Debugging defines
#define PRINTEVERYTHING 0
#define MAINBOARD 0

#define DEBUG 1

//Changeable constants
#define MESSAGETIMEOUT 3000
#define MAXDATASTRINGLEN 30
#define DEFAULTCHANNEL 13
#define CHECKSUMDIG 7 //Should be a prime number less than 78

//Pin Defintions
#define POTPIN A0

#define HC12TXPIN 10 //Might need to be pwm
#define HC12RXPIN 11 //Might need to be pwm
#define HC12SETPIN 12

#endif