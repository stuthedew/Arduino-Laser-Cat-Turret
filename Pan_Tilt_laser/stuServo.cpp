
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


void StuServo::setPowerPin( uint8_t powerPin ){

  _powerPin = powerPin ;
  pinMode( _powerPin, OUTPUT ) ;
  digitalWrite( _powerPin, HIGH ) ;

}


void StuServo::setCalibration( int min, int max ){
  _position.min = map(min, 0, 180, MG995_MIN_MICRO, MG995_MAX_MICRO);
  _position.max = map(max, 0, 180, MG995_MIN_MICRO, MG995_MAX_MICRO);
}

void StuServo::pause( void ){
  digitalWrite( _powerPin, LOW ) ;
}

void StuServo::wake( void ){
  digitalWrite( _powerPin, HIGH ) ;
}


void StuServo::stuWrite( int pos ){
  int position = map(pos, 0, 180, MG995_MIN_MICRO, MG995_MAX_MICRO);

  if( position < _position.min ){
    //write(_position.min) ;
    writeMicroseconds(_position.min) ;
  }
  else if( position > _position.max ){
    writeMicroseconds( _position.max );
  }
  else{
    writeMicroseconds( position ) ;
  }
  delay(5);

}


int StuServo::getMin( void ) const {
  return _position.min ;
}


int StuServo::getMax( void ) const {
  return _position.max ;
}
