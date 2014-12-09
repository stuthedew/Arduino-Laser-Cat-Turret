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
  LED_BLINK  ,
}ledState_e;

typedef struct led_t{

  uint8_t const
    pin ; // pin for the led

  bool
    state ; // current pin state (ON/OFF)

  Timer
    _blinkTimer;

  led_t( uint8_t ledPin ): pin( ledPin ), state( 0 ), _blinkTimer(){
    scheduler.addEvent(&_blinkTimer);
  }

}led_t;


class StuDisplay{

public:

  StuDisplay( uint8_t pwrLED, uint8_t contLED, uint8_t intLED ) ;

  void
    begin( void ) ,
    setLEDState( led_t* led, ledState_e e  ),
    setLEDState( uint8_t ledVal, ledState_e e ),
    setLEDStates( ledState_e e1, ledState_e e2, ledState_e e3 ),
    update( void ) ;

private:

  void
    _ledWrite( led_t* led, bool ledState ) ,
    _enableBlink( led_t* led, unsigned int duration, unsigned int onTime ) ,
    _blinkLED( led_t* led ) ,
    _disableBlink( led_t* led ) ;

  led_t* _led[ LED_NUMBER ] ;

  led_t
    _sleep        ,
    _continuous   ,
    _intermittent ;


    unsigned int
      _offTime,
      _blinkTime  ;

};
