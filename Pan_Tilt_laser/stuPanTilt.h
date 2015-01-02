
/**************************************************************************/
/*!
    @file     stuPanTilt.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for controlling dual servo pan-tilt rig
    (https://www.adafruit.com/products/1967) with an Arduino Uno (Atmel
    Atmega 328).


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#pragma once

#include "Arduino.h"
#include "stuServo.h"
#include <Time.h>
#include "stu_scheduler.h"
#include "panTilt_config.h"
#include "stu_display.h"
#include "stu_dial.h"
#include "stuLaser.h"
#include "SETTINGS.h"




  struct panTiltPos_t {
    int
      pos,
      angle,
      dir;

    const int
      minAngle,
      maxAngle,
      midOffset,
      midAngle,
      probOffset;


    panTiltPos_t( int mn, int mx, int mdOff = 0, int pbOff = 1): pos( 0 ), dir(1), minAngle(mn), maxAngle(mx), midOffset(mdOff), midAngle(((mx-mn) >>1) + mn + midOffset), probOffset(pbOff){}

  };


typedef enum {
  STATE_OFF,
  STATE_RUN,
  STATE_REST
}state_e;



typedef struct settings_t{
      state_e
        id;

      bool const
        laserState,
        servoState;

      Callback
        callback;

      ledState_e
        ledState[3];

      settings_t(bool laser, ledState_e e0, ledState_e e1, ledState_e e2, state_e e):laserState(laser), servoState(laser), id(e), callback(NULL){
        ledState[0] = e0;
        ledState[1] = e1;
        ledState[2] = e2;

      }
    } settings_t;


    typedef struct statePair_t{
      settings_t*
        state;

      time_t const
        duration;

      statePair_t(settings_t* s, time_t t=0): state(s), duration( t ){}

    };

  typedef struct mode_t{

      statePair_t
        settingA,
        settingB;

      statePair_t* currentSettings;
      statePair_t* nextSettings;


        mode_t(settings_t* s1, time_t duration1=0, settings_t* s2=NULL, time_t duration2=0 ):settingA(s1, duration1*60*1000), settingB(s2, duration2*60*1000), currentSettings(&settingA), nextSettings(&settingB){

        }
  };




  class PanTilt {

  public:

    PanTilt( uint8_t xPin, uint8_t yPin );

    void
      begin( void ),
      detach( void ),
      update( void ),
      shake( void ),
      setPosition(int X, int Y ),
      pause( unsigned long pauseVal, bool laserState = 1 ),
      setStateCallback(state_e e , Callback f) ;

    panTiltPos_t* getXPos( void );
    panTiltPos_t* getYPos( void );

    runmode_e
      getMode( void ) const;

    state_e
      getState( void ) const;

      Callback
        callback( void );

    Task* getTaskPtr( void );



      panTiltPos_t
        posX ,
        posY ;

  private:

    StuDisplay _display ;

    void
      _setMode( runmode_e mode ),
      _setState( settings_t* s );

    mode_t
      _offMode,
      _contMode,
      _intMode,
      _sleepMode;


    StuServo
      _xServo,
      _yServo;

    uint8_t
      _xPin,
      _yPin;

    void
      _updateAngles( void );



    mode_t*
      _modes[ 4 ];

    mode_t*
      _currentMode;

    Task
      _stateChangeTask;

    runmode_e
      _mode; // pan tilt mode

    state_e* const
      _currentState;

    StuDial
      _dial ;

    StuLaser
      _laser ;


  };
