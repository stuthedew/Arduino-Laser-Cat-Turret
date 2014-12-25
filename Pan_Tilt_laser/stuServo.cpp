
/**************************************************************************/
/*!
    @file     stuServo.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#include "stuServo.h"

void StuServo::calibrate(){


}

void StuServo::setCalibration(int min, int max){
  _position.min = min;
  _position.max = max;


}


void StuServo::stuWrite(int position){
  void StuServo::stuWrite( int position ){
    int curPos = read();
    int newPos;


    if( position < _position.min ){
      newPos = _position.min ;

    }
    else if( position > _position.max ){

      newPos = _position.max ;
    }
    else{

      newPos =  position ;
    }

    int diff = newPos - curPos;
    int sign = 1;
    if( diff < 0){
      sign = -1;
      diff *= sign;
    }

    while( curPos != newPos ){
      write( curPos );
      curPos += sign;
      delay(1);
    }


  }
}

int StuServo::getMin() const {
  return _position.min;
}

int StuServo::getMax() const {
  return _position.max;
}
