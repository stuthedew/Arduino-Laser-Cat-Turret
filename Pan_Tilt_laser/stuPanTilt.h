
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

  typedef struct{

    stateCallback callback ;

  }state_t;


  typedef struct {
    runmode_e id ;

    state_t* currentState ;

  } mode_t;


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

    mode_t
      _offMode  ,
      _contMode ,
      _intMode  ;

    state_t
      _offState   ,
      _runState   ,
      _restState  ,
      _sleepState ;


    runmode_e
      _mode; // pan tilt mode

    StuDial
      _dial ;

    StuLaser
      _laser ;

  };
