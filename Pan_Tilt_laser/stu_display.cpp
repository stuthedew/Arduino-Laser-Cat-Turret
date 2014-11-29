/**************************************************************************/
/*!
@file     stu_display.cpp
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Library to control leds displaying mode/power status


@section  HISTORY
v0.0.1 - First release

*/
/**************************************************************************/

#include "stu_display.h"


StuDisplay::StuDisplay( uint8_t pwrPin, uint8_t contPin, uint8_t intPin ):
                    _power( pwrPin ), _continuous( contPin ),
                    _intermittent( intPin ){

                      _led[ 0 ] = &_power ;
                      _led[ 1 ] = &_continuous ;
                      _led[ 2 ] = &_intermittent ;

}


void StuDisplay::begin( void ){

  for(int i = 0; i < LED_NUMBER; i++){
    pinMode( _led[ i ]->pin , OUTPUT ) ;
    _ledWrite( _led[ i ], LOW ) ;

  }

  update();

}

void StuDisplay::update( void ){
  //_setMode( Dial.getMode() ) ;

  for(int i = 0; i < LED_NUMBER; i++){
    digitalWrite(_led[ i ]->pin, _led[ i ]->state ) ;
  }

}

void StuDisplay::_ledWrite( led_t* led, bool ledState ){

  led->state = ledState ;

}

void StuDisplay::_setMode( runmode_e mode ){

  switch( mode ){

    case MODE_OFF:
      _ledWrite(&_power, LOW ) ;
      _ledWrite(&_continuous, LOW ) ;
      _ledWrite(&_intermittent, LOW ) ;
      break;

    case MODE_CONTINUOUS:
      _ledWrite(&_power, HIGH ) ;
      _ledWrite(&_continuous, HIGH ) ;
      _ledWrite(&_intermittent, LOW ) ;
      break;

    case MODE_INTERMITTENT:
      _ledWrite(&_power, HIGH ) ;
      _ledWrite(&_continuous, LOW ) ;
      _ledWrite(&_intermittent, HIGH ) ;
      break;

  }

}
