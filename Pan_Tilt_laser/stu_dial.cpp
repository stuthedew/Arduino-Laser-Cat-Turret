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



void StuDial::setPin( uint8_t dialPin ){
  _dialPin = dialPin ;

}

void StuDial::begin( void ){
  pinMode( _dialPin, INPUT ) ;
  StuDial::_update() ;
  //analogReference(INTERNAL);

}

void StuDial::_update( void ){

    //int adcReading = adcsample_and_lowpass( _dialPin, 1000, 300, 0.015, false );

    int minRead = 1023;
    int maxRead = 0;
    int sumRead = 0;

    for(int i = 0; i < 6; i++){
      int tmpRead = analogRead(_dialPin);
      minRead = min(tmpRead, minRead);
      maxRead = max(tmpRead, maxRead);
      sumRead += tmpRead;
      delay(5);
    }

    sumRead -= minRead;
    sumRead -= maxRead;
    sumRead += 2; // for integer rounding with shift
    int adcReading = sumRead >> 2; //divide by 4 (6 readings - hi - lo = 4 readings)


#ifdef SERIAL_DEBUG
#ifdef DIAL_DEBUG
  MY_SERIAL.println(F("Reading:"));
  MY_SERIAL.println(adcReading);
#endif
#endif

  if( adcReading <= ADC_VALUE_RANGE ){ // 0 == MODE_OFF
    _mode = MODE_OFF ;

  }else if( abs(adcReading - MAX_CONT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_CONTINUOUS ;
  }

  else if( abs(adcReading - MAX_INT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_INTERMITTENT ;

  }
  else if( adcReading >= MAX_SLEEP_ADC - ADC_VALUE_RANGE ){
    _mode = MODE_SLEEP ;

  }

}

runmode_e StuDial::getMode( void ){
  StuDial::_update();
  return _mode ;
}
