#pragma once

#include "Arduino.h"


//Embedded Arduino does not have Serial.
#ifndef EMBED
  #define BAUD_RATE 115200
#endif

#define DIRECTION_CHANGE_PROBABILITY 15


// Servo pins
#define X_PWR_PIN   A3
#define Y_PWR_PIN   A4
#define SERVO_X_PIN A0    // PWM Pins
#define SERVO_Y_PIN A1

//LED power pins
#define POWER_PIN 8 // Power lED pin
#define CONT_PIN  4 // Continuous LED pin
#define INT_PIN   3 // Intermittent LED pin

#define LASER_PIN 9

#define DIAL_PIN A2


// Global Run Mode enum
typedef enum {
  MODE_OFF = 0      ,
  MODE_CONTINUOUS   ,
  MODE_INTERMITTENT ,
  MODE_SLEEP
}runmode_e;
