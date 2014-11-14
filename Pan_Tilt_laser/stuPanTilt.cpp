
/**************************************************************************/
/*!
    @file     stuPanTilt.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for controlling dual servo pan-tilt rig
    (https://www.adafruit.com/products/1967) with an Arduino Uno (Atmel
    Atmega 328).


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#include "stuPanTilt.h"

//namespace stu{

  PanTilt::PanTilt(uint8_t xPin, panTiltPos_t *xPos, uint8_t yPin, panTiltPos_t *yPos):_xServo(), _yServo(){

    _xPin = xPin;
    _yPin = yPin;

    _Xpos = xPos;
    _Ypos = yPos;

    _xServo.setCalibration(_Xpos->minAngle, _Xpos->maxAngle);
    _yServo.setCalibration(_Ypos->minAngle, _Ypos->maxAngle);


  }

  void PanTilt::begin(){
    _xServo.attach(_xPin);
    _yServo.attach(_yPin);
    delay(500);
  }

  void PanTilt::detach(){
    _xServo.detach();
    _yServo.detach();
  }

  void PanTilt::updateAngles(){
    _update();

  }

  void PanTilt::_update(){
    static int oldXangle, oldYangle;


    _xServo.stuWrite(_Xpos->angle);
    delay(10);
    _yServo.stuWrite(_Ypos->angle);
    delay(abs((_Xpos->angle - oldXangle)+15));
    delay(abs((_Ypos->angle - oldYangle)+15));

    oldXangle = _Xpos->angle;
    oldYangle = _Ypos->angle;

  }

//}// end namespace
