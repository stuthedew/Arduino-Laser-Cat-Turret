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
                    _intermittent( intPin ),_blinkTime(0){

                      _led[ 0 ] = &_power ;
                      _led[ 1 ] = &_continuous ;
                      _led[ 2 ] = &_intermittent ;
                      _currentDisplayMode = MODE_SLEEP;

}


void StuDisplay::begin( void ){

  scheduler.addEvent(&_blinkTimer);



  for(int i = 0; i < LED_NUMBER; i++){
    pinMode( _led[ i ]->pin , OUTPUT ) ;
    _ledWrite( _led[ i ], HIGH );

  }

  StuDisplay::update();
  delay( 450 );

  StuDisplay::setMode( MODE_OFF );
  delay(1000);

}

void StuDisplay::update( void ){

  for(int i = 0; i < LED_NUMBER; i++){
    digitalWrite(_led[ i ]->pin, _led[ i ]->state ) ;
  }

  if(_blinkTimer.enabled()){
    if(_blinkTimer.check(ELAPSE_DISABLE)){
      #ifndef EMBED
        Serial.println(F("Blink Timer"));
      #endif
      _blinkLED();
    }
  }
}



void StuDisplay::setMode( runmode_e mode ){

  if(mode != _currentDisplayMode){
    #ifndef EMBED
      Serial.println(F("CHANGE DISPLAY MODE"));
    #endif
    switch( mode ){

      case MODE_OFF:
        _disableBlink();
        _ledWrite(&_power, LOW ) ;
        _ledWrite(&_continuous, LOW ) ;
        _ledWrite(&_intermittent, LOW ) ;
        break;

      case MODE_CONTINUOUS:
        _disableBlink();
        _ledWrite(&_power, HIGH ) ;
        _ledWrite(&_continuous, HIGH ) ;
        _ledWrite(&_intermittent, LOW ) ;
        break;

      case MODE_INTERMITTENT:
        _disableBlink();
        _ledWrite(&_power, HIGH ) ;
        _ledWrite(&_continuous, LOW ) ;
        _ledWrite(&_intermittent, HIGH ) ;
        break;

      case MODE_REST:
        _ledWrite(&_power, HIGH ) ;
        _ledWrite(&_continuous, LOW ) ;
        _enableBlink( &_intermittent, 500, 500 ) ; //set intermittent led to blink every half second
        break;

      case MODE_SLEEP:
        _enableBlink( &_power, 10000, 350 ) ; //set power led to blink every 10 seconds
        _ledWrite(&_continuous, LOW ) ;
        _ledWrite(&_intermittent, LOW ) ;

        break;


    }
    StuDisplay::update();
  }

}


void StuDisplay::_ledWrite( led_t* led, bool ledState ){

  led->state = ledState ;

}

void StuDisplay::_enableBlink( led_t* led, unsigned int interval, unsigned int onTime ){
  _ledToBlink = led ;
  _offTime = interval ;
  _blinkTime = onTime;

  _blinkTimer.setInterval( _offTime ) ;
  _blinkTimer.start();

}

void StuDisplay::_disableBlink( void ){
  _blinkTimer.stop();

}

void StuDisplay::_blinkLED( void ){
  static uint8_t intervalItr;
  unsigned int* interval[2];

  interval[ 0 ] = &_offTime;
  interval[ 1 ] = &_blinkTime;
    _ledWrite(_ledToBlink, intervalItr) ;
    StuDisplay::update() ;

    _blinkTimer.setInterval(*interval[intervalItr]);

    _blinkTimer.resetPeriodic();

    intervalItr ^= 1;


}
