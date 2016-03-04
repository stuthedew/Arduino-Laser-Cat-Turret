
/*****************************************************************************!
    @file     stuServo.cpp                                                   *
    @author   Stuart Feichtinger                                             *
    @license  MIT (see license.txt)                                          *
 *
    Augmented servo library based off of Arduino Servo library. Adds maximum *
    and minimum angles, and prevents servo from going past them.             *
 *
 *
    @section  HISTORY                                                        *
    v1.0 - First release                                                     *
 *
 *****************************************************************************/


#include "stuServo.h"


void StuServo::calibrate() {}

void StuServo::setCalibration(int min, int max) {
  _position.min = min;
  _position.max = max;
}

void StuServo::stuWrite(int position) {
  if (position < _position.min) {
    write(_position.min);
  }
  else if (position > _position.max) {
    write(_position.max);
  }
  else {
    write(position);
  }
}

int StuServo::getMin() {
  return _position.min;
}

int StuServo::getMax() {
  return _position.max;
}
