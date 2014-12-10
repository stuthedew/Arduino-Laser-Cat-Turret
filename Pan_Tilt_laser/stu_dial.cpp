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


  StuDial Dial;


void StuDial::setPin( uint8_t dialPin ){
  _dialPin = dialPin ;

}

void StuDial::begin( void ){
  pinMode( _dialPin, INPUT ) ;
  update() ;

}

void StuDial::update( void ){
  int adcReading = 0;

    analogReference(INTERNAL); //set analog reference to internal 1.1V
    adcReading = analogRead( _dialPin ) * 1100/1024 ; //scale to adjust for Aref
    analogReference(DEFAULT);


#ifndef EMBED
  Serial.println(F("Reading:"));
  Serial.println(adcReading);
#endif

  if( abs(adcReading - MAX_OFF_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_OFF ;

  }else if( abs(adcReading - MAX_CONT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_CONTINUOUS ;
  }

  else if( abs(adcReading - MAX_INT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_INTERMITTENT ;

  }
  else
    {
      _mode = MODE_INTERMITTENT ;
  }

}

runmode_e StuDial::getMode( void ) const{
  return _mode ;
}
