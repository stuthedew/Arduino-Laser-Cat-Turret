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



#define VALUE_RANGE 2
#define ADC_SAMPLES 3

#define ADC_VALUE_RANGE VALUE_RANGE // * ADC_SAMPLES

// ADC readings from selecter potentiometer
#define MAX_OFF_ADC   1023 // * ADC_SAMPLES // If less than, mode = OFF
#define MAX_CONT_ADC  990 // * ADC_SAMPLES // If less than, mode = CONTINUOUS
#define MAX_INT_ADC  863 // * ADC_SAMPLES // If less than, mode = INTERMITTENT
#define MAX_SLEEP_ADC  743 // * ADC_SAMPLES // If less than, mode = INTERMITTENT


class StuDial{

public:

  void
    begin( void ) ,
    setPin( uint8_t pin ) ,
    update( void ) ;

  runmode_e
    getMode( void ) const;


private:
    uint8_t
      _dialPin ;

    runmode_e
      _mode ;

};

extern StuDial Dial;
