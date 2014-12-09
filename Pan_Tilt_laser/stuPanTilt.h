
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


#define DEFAULT_MIN 5
#define DEFAULT_MAX 170

typedef void (*stateCallback)(void);


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
/*
  typedef struct state1_t{
      bool const
        laserState;

      ledState_e const
        ledState[3];

      state_t( bool laser, ledState_e e1, ledState_e e2, ledState_e e3 ):laserState( laser ), ledState[0]( e1 ), ledState[1](e2), ledState[2](e3){}

  };

*/

typedef struct state_t{

      bool const
        laserState;

      ledState_e
        ledState[3];

      state_t(bool laser, ledState_e e0, ledState_e e1, ledState_e e2):laserState(laser){
        ledState[0] = e0;
        ledState[1] = e1;
        ledState[2] = e2;

      }
    } state_t;


    typedef struct statePair_t{
      state_t*
        state;

      time_t const
      duration;

      statePair_t(state_t* s, time_t t=0): state(s), duration( t ){}

    };

  typedef struct mode_t{

      statePair_t
        stateA,
        stateB;

      statePair_t* currentState;
      statePair_t* nextState;


        mode_t(state_t* s1, time_t duration1=0, state_t* s2=NULL, time_t duration2=0 ):stateA(s1, duration1), stateB(s2, duration2), currentState(&stateA), nextState(&stateB){}
  };




  class PanTilt {

  public:

    PanTilt( uint8_t xPin, uint8_t yPin );

    void
      begin( void ),
      detach( void ),
      setMode( runmode_e mode ),
      update( void ),
      shake( void ),
      setPosition(int X, int Y );

    panTiltPos_t* getXPos( void );
    panTiltPos_t* getYPos( void );

    runmode_e
      getMode( void ) const;

      panTiltPos_t
        posX ,
        posY ;

  private:

    StuDisplay _display ;

    void _setState( state_t* s );

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

    Timer
      _stateChangeTimer;

    runmode_e
      _mode; // pan tilt mode

    StuDial
      _dial ;

    StuLaser
      _laser ;

    Callback
      _callback();

  };
