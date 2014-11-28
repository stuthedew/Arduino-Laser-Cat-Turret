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



// ADC readings from selecter potentiometer
#define MAX_OFF_ADC   50  // If less than, mode = OFF
#define MAX_CONT_ADC  150 // If less than, mode = CONTINUOUS
#define MAX_INT_ADC   250 // If less than, mode = INTERMITTENT

namespace stu{

typedef enum{
  MODE_OFF = 0,
  MODE_CONTINUOUS,
  MODE_INTERMITTENT

}potmode_e;


class StuDial{

public:

  void        begin( void ) ;
  void        setPin( uint8_t pin ) ;
  void        update( void ) ;

  potmode_e   getMode( void ) const;


private:
    uint8_t _dialPin ;

    potmode_e _mode ;

};

extern StuDial Dial;

}
