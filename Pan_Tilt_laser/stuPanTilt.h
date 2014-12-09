
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

  typedef struct state_t{
      const bool
        laserState;

      const ledState_e
        ledState;

      state_t( bool laser, ledState_e e ):laserState( laser ), ledState( e ){}

  };

  static state_t
    off_state( 0, LED_OFF ) ,
    on_state( 1, LED_ON ) ,
    rest_state( 0, LED_BLINK );



    typedef struct linkedstate_t{

      state_t const*
        currentState;
      const time_t
        timeToNextState;
      linkedstate_t*
        nextState;

      linkedstate_t(state_t* curState, time_t tmToNxt ):currentState( curState ), timeToNextState( tmToNxt * 60 * 1000 ), nextState( NULL ){}
    }linkedstate_t;


  typedef struct mode_t{

      linkedstate_t
        stateA ,
        stateB ;

      mode_t(state_t* StateAPtr, time_t StateA_duration, state_t* StateBPtr, time_t StateB_duration ):stateA(StateAPtr, StateA_duration), stateB( StateBPtr , StateB_duration ){
        stateA.nextState = &stateB;
      }
      mode_t(state_t* StateAPtr):stateA( StateAPtr, 0 ), stateB( NULL , 0 ){
      }

  } ;




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

    runmode_e
      getMode( void ) const;

  private:



    StuDisplay _display ;

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

    panTiltPos_t
      _posX ,
      _posY ;


    mode_t*
      _modes[ 4 ];

    mode_t
      _currentMode;


    runmode_e
      _mode; // pan tilt mode

    StuDial
      _dial ;

    StuLaser
      _laser ;

  };
