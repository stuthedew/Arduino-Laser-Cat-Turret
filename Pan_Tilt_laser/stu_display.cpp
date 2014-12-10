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


StuDisplay::StuDisplay( uint8_t contPin, uint8_t intPin, uint8_t sleepPin ):
                    _sleep( sleepPin ), _continuous( contPin ),
                    _intermittent( intPin ),_blinkTime(0){


                      _led[ 0 ] = &_continuous ;
                      _led[ 1 ] = &_intermittent ;
                      _led[ 2 ] = &_sleep ;
}


void StuDisplay::begin( void ){


  for(int i = 0; i < LED_NUMBER; i++){
    scheduler.addEvent(&_led[ i ]->_blinkTimer);
    pinMode( _led[ i ]->pin , OUTPUT ) ;
    _ledWrite( _led[ i ], HIGH );

  }

  StuDisplay::update();

  delay(450);

  for(int i = 0; i < LED_NUMBER; i++){
    _ledWrite( _led[ i ], LOW );

  }
  StuDisplay::update();

  delay(1000);

}

void StuDisplay::update( void ){

  for(int i = 0; i < LED_NUMBER; i++){
    digitalWrite(_led[ i ]->pin, _led[ i ]->state ) ;

    if(_led[ i ]->_blinkTimer.enabled()){
      if(_led[ i ]->_blinkTimer.check()){
        #ifndef EMBED
          Serial.println(F("Blink Timer"));
        #endif
        _blinkLED( _led[ i ] );
      }
    }
  }
}


void StuDisplay::setLEDStates( ledState_e e1, ledState_e e2, ledState_e e3 ){

  setLEDState( &_continuous , e1 );
  setLEDState( &_intermittent , e2 );
  setLEDState( &_sleep , e3 );

}

void StuDisplay::setLEDState( uint8_t ledVal, ledState_e e ){
    return setLEDState( _led[ledVal], e );

}



void StuDisplay::setLEDState( led_t* l, ledState_e e ){

    switch( e ){

      case LED_OFF:
        _ledWrite(l, LOW ) ;
        break;

      case LED_ON:
        _ledWrite(l, HIGH ) ;
        break;

      case LED_BLINK:
      _enableBlink(l, 100, 400 ) ;
        break;


    }

  }


void StuDisplay::_ledWrite( led_t* led, bool ledState ){

  led->state = ledState ;

}

void StuDisplay::_enableBlink( led_t* led, unsigned int interval, unsigned int onTime ){
  _offTime = interval ;
  _blinkTime = onTime;

  led->_blinkTimer.setInterval( _offTime ) ;
  led->_blinkTimer.start();

}

void StuDisplay::_disableBlink( led_t* led ){
  led->_blinkTimer.stop();

}

void StuDisplay::_blinkLED( led_t* led ){
  static uint8_t intervalItr;
  unsigned int* interval[2];

  interval[ 0 ] = &_offTime;
  interval[ 1 ] = &_blinkTime;
    _ledWrite(led, intervalItr) ;
    StuDisplay::update() ;

    led->_blinkTimer.setInterval(*interval[intervalItr]);

    led->_blinkTimer.resetPeriodic();

    intervalItr ^= 1;

}
