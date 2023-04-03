#include "actuatingCases.h"
int actuatingCases(Message message) {


  if (message.sensor == pot) {
    int servoPosition = map(message.data.data.intData, 0, 1023, 0, 180);
    return servoPosition;
  }
  
  if(message.sensor == photo){
    int ledCase = message.data.data.intData;
    if(ledCase<5){
      return 1;
    }
    else{
      return 0;
    }
  }
}