/**************************************************************************/
/*!
@file     stu_dial.h
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Library to read input from potentiometer and return appropriate run mode.


@section  HISTORY
v0.0.1 - First release

*/
/**************************************************************************/

#pragma once

#include "Arduino.h"
#include "panTilt_config.h"


#define DIAL_DEBUG

#define ADC_VALUE_RANGE 7

// ADC readings from selecter potentiometer
#define MAX_OFF_ADC   0 // * ADC_SAMPLES // If less than, mode = OFF
#define MAX_CONT_ADC  35 // * ADC_SAMPLES // If less than, mode = CONTINUOUS
#define MAX_INT_ADC  165 // * ADC_SAMPLES // If less than, mode = INTERMITTENT
#define MAX_SLEEP_ADC  295 // * ADC_SAMPLES // If less than, mode = INTERMITTENT


class StuDial{

public:

  void
    begin( void ) ,
    setPin( uint8_t pin ) ;


  runmode_e
    getMode( void );


private:
    uint8_t
      _dialPin ;

    void
      _update( void ) ;

    runmode_e
      _mode ;

};
