/*************************************************************************!
    @file     stuLaser.h                                                 *
    @author   Stuart Feichtinger                                         *
    @license  MIT (see license.txt)                                      *
                                                                         *
    Simple library for controlling laser diode                           *
    (https://www.adafruit.com/products/1054) with NPN Transistor (e.g.   *
    BC547, PN2222 {https://www.adafruit.com/products/756}) as a low-side *
    switch.                                                              *
                                                                         *
                                                                         *
    @section  HISTORY                                                    *
    v1.0 - First release                                                 *
                                                                         *
 *************************************************************************/


#ifndef _STULASER_H_
#define _STULASER_H_

#include "Arduino.h"
#include <AVector.h>
#include <math.h>


struct angle_t {
  float
    hRad,
    vRad;

  int
    hDeg,
    vDeg;
};

class StuLaser {
public:

  StuLaser(uint8_t laserPin);

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


#endif /* ifndef _STULASER_H_ */
