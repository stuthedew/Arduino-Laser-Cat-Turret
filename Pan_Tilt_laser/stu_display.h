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
#include "panTilt_config.h"
#include "stu_scheduler.h"


#define LED_NUMBER 3


typedef enum ledState_e{
  LED_OFF = 0,
  LED_ON     ,
  LED_BLINK
}ledState_e;

typedef struct led_t{

  uint8_t const
    pin ; // pin for the led

  bool
    state ; // current pin state (ON/OFF)

  led_t( uint8_t ledPin ): pin( ledPin ), state( 0 ){}

}led_t;


class StuDisplay{

public:

  StuDisplay( uint8_t pwrLED, uint8_t contLED, uint8_t intLED ) ;

  void
    begin( void ) ,
    setLEDStates( ledState_e e1, ledState_e e2, ledState_e e3 ),
    update( void ) ;

private:

  void
    _setState( led_t* led, ledState_e e ),
    _ledWrite( led_t* led, bool ledState ) ,
    _enableBlink( led_t* led, unsigned int duration, unsigned int onTime ) ,
    _blinkLED( void ) ,
    _disableBlink( void ) ;

  led_t* _led[ LED_NUMBER ] ;
  led_t* _ledToBlink ;

  led_t
    _sleep        ,
    _continuous   ,
    _intermittent ;

    Timer
      _blinkTimer;

    unsigned int
      _offTime,
      _blinkTime  ;

};
