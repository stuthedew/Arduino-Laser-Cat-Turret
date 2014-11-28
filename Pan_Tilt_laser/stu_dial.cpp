/**************************************************************************/
/*!
@file     stu_dial.cpp
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Library to read input from potentiometer and return appropriate run mode.


@section  HISTORY
v0.0.1 - First release

*/
/**************************************************************************/

#include "stu_dial.h"

namespace stu{

  StuDial Dial;


void StuDial::setPin( uint8_t dialPin ){
  _dialPin = dialPin ;

}

void StuDial::begin( void ){
  pinMode( _dialPin, INPUT ) ;
  update() ;

}

void StuDial::update( void ){

  int adcReading = analogRead( _dialPin ) ;

  if( adcReading < MAX_OFF_ADC ){
    _mode = MODE_OFF ;

  }else if( adcReading < MAX_CONT_ADC ){
    _mode = MODE_CONTINUOUS ;
  }
  _mode = MODE_INTERMITTENT ;

}

potmode_e StuDial::getMode( void ) const{
  return _mode ;

}



}
