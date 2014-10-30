
/**************************************************************************/
/*!
    @file     stuPanTilt.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for controlling dual servo pan-tilt rig
    (https://www.adafruit.com/products/1967) with an Arduino Uno (Atmel
    Atmega 328).


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/


#ifndef _STUPANTILT_H_
#define _STUPANTILT_H_

#include "stuServo.h"
#include <Servo.h>
#include "Arduino.h"
#include <math.h>
#include "stu_vector.h"

#define DEFAULT_MIN 5
#define DEFAULT_MAX 170

struct panTiltPos_t {
  int
    angle,
    dir,
    pbOffset,
    midOffset;

  const int
    minAngle,
    maxAngle,
    midAngle;


  panTiltPos_t(int mn, int mx):dir(1), minAngle(mn), maxAngle(mx),
    midAngle(((mx-mn) >>1) + mn){}

};


class PanTilt {

public:

  PanTilt(uint8_t xPin, panTiltPos_t *xPos, uint8_t yPin, panTiltPos_t *yPos);

  void
    begin(),
    detach(),
    updateAngles(),
    setHeight(int);

private:

  StuServo
    _xServo,
    _yServo;

  uint8_t
    _xPin,
    _yPin;

  void
    _update();

  panTiltPos_t
    *_Xpos,
    *_Ypos;

  SVector
    _hPos,
    _vPos;

};



#endif
