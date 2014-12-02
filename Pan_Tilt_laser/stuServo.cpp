
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
  digitalWrite( _powerPin, LOW ) ;

}


void StuServo::setCalibration( int min, int max ){
  _position.min = min;
  _position.max = max;
}


void StuServo::stuWrite( int position ){

  digitalWrite( _powerPin, HIGH ) ;

  if( position < _position.min ){
    write(_position.min) ;
  }
  else if( position > _position.max ){
    write( _position.max );
  }
  else{
    write( position ) ;
  }
  digitalWrite( _powerPin, LOW ) ;
}


int StuServo::getMin( void ) const {
  return _position.min ;
}


int StuServo::getMax( void ) const {
  return _position.max ;
}
