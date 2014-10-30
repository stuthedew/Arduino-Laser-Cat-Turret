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
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STULASER_H_
#define _STULASER_H_

#include "Arduino.h"
#include "stu_vector.h"

class StuLaser {

public:

  StuLaser(uint8_t laserPin);

  void
    begin(),
    fire(boolean state),
    setOrigin(int vX, int vY),
    setPosition(int hX, int hY);


  int
    vX(),
    vY(),
    hX(),
    hY();

  float
    vAngle();

private:

  uint8_t
    _pin;

  SVector
    _vDot,
    _hDot,
    _hOrigin,
    _vOrigin;

};


#endif
