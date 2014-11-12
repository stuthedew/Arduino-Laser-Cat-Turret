
/**************************************************************************/
/*!
    @file     stuServo.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/


#pragma once

#include <Servo.h>


struct servoPos{
  int
    min,
    max,
    current;

};

class StuServo: public Servo {

public:

  void
    setCalibration(int min, int max),
    calibrate(),
    stuWrite(int position);

  int
    getMin() const ,
    getMax() const ;


private:

  servoPos
    _position;



};
