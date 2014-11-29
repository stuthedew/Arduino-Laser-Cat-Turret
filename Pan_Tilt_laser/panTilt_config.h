#pragma once

#define BAUD_RATE 115200


#include "Arduino.h"



#define MS_SWITCH_PIN 3
#define MS_LED_PIN 4


#define POWER_PIN 7
#define CONT_PIN  8
#define INT_PIN   9

#define LASER_PIN 5


#define SERVO_X_PIN A0
#define SERVO_Y_PIN A1
#define DIAL_PIN A2

#define DIRECTION_CHANGE_PROBABILITY 15

typedef enum {
  MODE_OFF = 0      ,
  MODE_CONTINUOUS   ,
  MODE_INTERMITTENT ,
  MODE_SLEEP

}runmode_e;
