/**************************************************************************/
/*!
    @file     stuLaser.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for controlling laser diode
    (https://www.adafruit.com/products/1054) with NPN Transistor (e.g.
    BC547, PN2222 {https://www.adafruit.com/products/756}) as a low-side
    switch.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#pragma once

#include "Arduino.h"
#include <AVector.h>
#include <math.h>


struct angle_t{
  float
    hRad,
    vRad;

  int
    hDeg,
    vDeg;

};

class StuLaser {

public:

  StuLaser( uint8_t laserPin );

<<<<<<< HEAD
  void
    begin(),
    fire(boolean state),
    setOrigin(int vX, int vY),
    setDotPosition(int hX, int hY),
    setDotPositionFast(int hX, int hY);

    void
      calcAngles();


  int
    vX(),
    vY(),
    hX(),
    hY(),
    vAngleDeg(),
    hAngleDeg();

  float
    vAngle(),
    hAngle();

  inline int
    rad2Deg(float);

private:

  uint8_t
    _pin;

  AVector
    _vDot,
    _hDot,
    _hOrigin,
    _vOrigin;

  angle_t
    _servoAngle;





};
=======
    void
      begin( void ) ,
      fire( boolean state ) ;

private:
>>>>>>> master

    uint8_t
      _pin ;

};
