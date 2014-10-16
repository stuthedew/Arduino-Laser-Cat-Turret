
/**************************************************************************/
/*!
    @file     stuPanTilt.cpp
    @author   Stuart Feichtinger
    @license  BSD (see license.txt)

    Library for controlling dual servo pan-tilt rig
    (https://www.adafruit.com/products/1967) with an Arduino Uno (Atmel
    Atmega 328).


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/


#include "stuPanTilt.h"



PanTilt::PanTilt(uint8_t xPin, panTiltPos_t *xPos, uint8_t yPin, panTiltPos_t *yPos, int height):_xServo(), _yServo(), _height(height){

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

void PanTilt::_setAnglesFromPosition(){
  _Xpos->angle = _rad2Deg(atan(_Ypos->pos/_Xpos->pos));
  _Ypos->angle = _rad2Deg(atan(sqrt(square(_Ypos->pos) + square(_Xpos->pos)) /_height));
}

void PanTilt::_setPositionFromAngles(){

  float adjacent = _height / tan(_deg2Rad(90-_Ypos->angle));
  _Ypos->pos = sin(_deg2Rad(_Xpos->angle)) * adjacent;
  _Xpos->pos = cos(_deg2Rad(_Xpos->angle)) * adjacent;
}

void PanTilt::updateAngles(){
  _update();

}

void PanTilt::_update(){
  static int oldXangle, oldYangle;


  _xServo.stuWrite(_Xpos->angle);
  delay(15);
  _yServo.stuWrite(_Ypos->angle);
  delay(abs(2*(_Xpos->angle - oldXangle)+15));
  delay(abs(2*(_Ypos->angle - oldYangle)+15));

  oldXangle = _Xpos->angle;
  oldYangle = _Ypos->angle;

}

float PanTilt::_rad2Deg(float radVal){
  #define RAD2DEG_CONVERSION_FACTOR 57.2958

  return radVal * RAD2DEG_CONVERSION_FACTOR;
}

float PanTilt::_deg2Rad(float degVal){
  #define DEG2RAD_CONVERSION_FACTOR 0.01745

  return degVal * DEG2RAD_CONVERSION_FACTOR;
}
