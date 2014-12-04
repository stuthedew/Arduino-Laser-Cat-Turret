
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
#include "stu_scheduler.h"
#include "panTilt_config.h"
#include "stu_display.h"
#include "stu_dial.h"


#define DEFAULT_MIN 5
#define DEFAULT_MAX 170

typedef void (*modeCallback)(void);

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


  class PanTilt {

  public:

    PanTilt( uint8_t xPin, uint8_t yPin );

    void
      begin( void ),
      detach( void ),
      setMode( runmode_e mode ),
      update( void ),
      shake( void );

    runmode_e
      getMode( void ) const;

  private:

    typedef struct{
      runmode_e id ;
      modeCallback callback ;

    }mode_t;

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
      offMode   ,
      contMode  ,
      intMode   ;


    runmode_e
      _mode; // pan tilt mode

    StuDial
      _dial ;

  };
