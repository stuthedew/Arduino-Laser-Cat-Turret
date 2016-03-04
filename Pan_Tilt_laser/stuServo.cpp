
/**************************************************************************/
/*!
@file     stuServo.cpp
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Augmented servo library based off of Arduino Servo library. Adds maximum
and minimum angles, and prevents servo from going past them.


@section  HISTORY
v0.0.1 - First release
v0.0.2 - Switched write to writeMicroseconds to allow maximal servo
rotation.
v0.0.3 - Added 5 microsecond delay after wake to allow capacitors to charge.

*/
/**************************************************************************/

#include "stuServo.h"


void StuServo::setPowerPin( uint8_t powerPin ){

  _powerPin = powerPin ;
  pinMode( _powerPin, OUTPUT ) ;
  digitalWrite( _powerPin, HIGH ) ;

}


void StuServo::setCalibration( int min, int max ){
  _position.min = min;
  _position.max = max;

<<<<<<< HEAD
=======
}

void StuServo::pause( void ){
  digitalWrite( _powerPin, LOW ) ;
}

void StuServo::wake( void ){
  digitalWrite( _powerPin, HIGH ) ;
  delay(10);
>>>>>>> master
}


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


int StuServo::getMin( void ) const {
  return _position.min ;
}


int StuServo::getMax( void ) const {
  return _position.max ;
}
