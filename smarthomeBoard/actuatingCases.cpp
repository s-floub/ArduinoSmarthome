#include "actuatingCases.h"

//Function to handle servo and LED cases
int actuatingCases(Message message) {

  //If the sensor is a potentiometer
  if (message.sensor == pot) {
    //Map the potentiometer analog reading to a position between 0 and 180 degrees
    int servoPosition = map(message.data.data.intData, 0, 1023, 0, 180);
    //Return this  as the position to move the servo to
    return servoPosition;
  }
//If the sensor is a photoresistor
  if (message.sensor == photo) {
    //Get the photoresistor reading as a lux value
    int ledCase = message.data.data.intData;

    //If the lux reading is below 5
    if (ledCase < 5) {
      return 1; //return 1, which will turn the light on
    } else {
      //Otherwise, the lux value is greater, meaning it is bright, so the light is turned off
      return 0; 
    }
  }
}