#pragma once

#include "Arduino.h"

#ifndef EMBED
  #define BAUD_RATE 115200
#endif

#define DIRECTION_CHANGE_PROBABILITY 15


// Servo pins
#define X_PWR_PIN   99    // TODO: define servo power pins
#define Y_PWR_PIN   99    // TODO: Set pins for Pro trinket
#define SERVO_X_PIN A0    // PWM Pins
#define SERVO_Y_PIN A1

//LED power pins
#define POWER_PIN 7 // Power lED pin        TODO: Set pins for Pro trinket
#define CONT_PIN  8 // Continuous LED pin   TODO: Set pins for Pro trinket
#define INT_PIN   9 // Intermittent LED pin TODO: Set pins for Pro trinket

#define LASER_PIN 5 // TODO: Set pins for Pro trinket

#define DIAL_PIN A2


// Global Run Mode enum
typedef enum {
  MODE_OFF = 0      ,
  MODE_CONTINUOUS   ,
  MODE_INTERMITTENT ,
  MODE_SLEEP
}runmode_e;
