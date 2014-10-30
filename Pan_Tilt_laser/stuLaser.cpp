
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
  _vOrigin = _vOrigin.set(vX, vY);
}

void StuLaser::fire(boolean state){
  digitalWrite(_pin, state);

}

void StuLaser::setPosition(int hX, int hY){
  _hDot = _hDot.set(hX, hY);
  _vDot = _vDot.set(_hDot.mag(), 0);
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
  return _vDot.angleBetween(&_vOrigin);
}
