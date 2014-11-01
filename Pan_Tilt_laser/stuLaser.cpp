
/**************************************************************************/
/*!
    @file     stuLaser.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for controlling laser diode
    (https://www.adafruit.com/products/1054) with NPN Transistor (e.g.
    BC547, PN2222 {https://www.adafruit.com/products/756}) as a low-side
    switch.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/



#include "stuLaser.h"
#define M_PI_2   1.57079632679489661923
#define RAD_2_DEG_CONST 57.30

StuLaser::StuLaser(uint8_t pin){
  _pin = pin;

}

void StuLaser::begin(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);


}

void StuLaser::setOrigin(int vX, int vY){
  _vDot.set(0, -1*vY);
  _vOrigin.set(0, -1*vY);
}

void StuLaser::fire(boolean state){
  digitalWrite(_pin, state);

}

void StuLaser::setDotPosition(int hX, int hY){
  _hDot.set(hX, hY);
  _vDot.set(_hDot.mag(), _vDot.y());
  _hOrigin.set(hX, 0);

  _servoAngle.vRad = M_PI_2 - _vDot.angleBetween(&_vOrigin);
  _servoAngle.hRad = M_PI_2 + _hDot.angleBetween(&_hOrigin);
  _servoAngle.hDeg = RAD_2_DEG_CONST * _servoAngle.hRad;
  _servoAngle.vDeg = RAD_2_DEG_CONST * _servoAngle.vRad;

}

void StuLaser::setDotPositionFast(int hX, int hY){
  _hDot.set(hX, hY);
  _vDot.set(_hDot.mag(), _vDot.y());
  _hOrigin.set(hX, 0);

  //_servoAngle.hRad = M_PI_2 + _hDot.angleBetweenFast(&_hOrigin);
  //_servoAngle.vRad = M_PI_2 - _vDot.angleBetweenFast(&_vOrigin);

  _servoAngle.hDeg = 90 + _hDot.angleBetweenFast(&_hOrigin);
  _servoAngle.vDeg = 90 - _vDot.angleBetweenFast(&_vOrigin);

}

int StuLaser::hX(){
  return _hDot.x();
}

int StuLaser::hY(){
  return _hDot.y();

}

int StuLaser::vX(){
  return _vDot.x();

}

int StuLaser::vY(){
  return _vDot.y();

}

float StuLaser::vAngle(){
  return _servoAngle.vRad;
}

int StuLaser::vAngleDeg(){

  return _servoAngle.vDeg;
}

float StuLaser::hAngle(){
return _servoAngle.hRad;
}

int StuLaser::hAngleDeg(){
return _servoAngle.hDeg;
}

void StuLaser::calcAngles(){
  _servoAngle.hRad = M_PI - asin(_hDot.y()/_hDot.y());
  _servoAngle.vRad = M_PI_2 - atan(_hDot.mag()/(-1*_vOrigin.y()));
  _servoAngle.hDeg = RAD_2_DEG_CONST * _servoAngle.hRad;
  _servoAngle.vDeg = RAD_2_DEG_CONST * _servoAngle.vRad;
}
