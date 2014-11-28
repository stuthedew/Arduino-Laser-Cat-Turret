/**************************************************************************/
/*!
@file     stu_display.h
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Library to control leds displaying mode/power status


@section  HISTORY
v0.0.1 - First release

*/
/**************************************************************************/

#pragma once

#include "Arduino.h"
#include "stu_dial.h"

#define LED_NUMBER 3


namespace stu{

typedef struct led_t{

  uint8_t const pin  ; // pin for the led
  bool          state   ; // current pin state (ON/OFF)

  led_t( uint8_t ledPin ): pin( ledPin ), state( 0 ){}

}led_t;


class StuDisplay{

public:

  StuDisplay( uint8_t pwrLED, uint8_t contLED, uint8_t intLED ) ;

  void  begin( void ) ;
  void  update( void ) ;

private:

  void _ledWrite( led_t* led, bool ledState ) ;

  void _setMode( potmode_e mode ) ;

  led_t*        _led[ LED_NUMBER ] ;

  led_t
    _power        ,
    _continuous   ,
    _intermittent ;

};

}
