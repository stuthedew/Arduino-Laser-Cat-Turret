#ifndef _STULASER_H_
#define _STULASER_H_

#include "Arduino.h"

class StuLaser {

public:

  StuLaser(uint8_t laserPin);

  void
    begin(),
    fire(boolean state);

private:
  uint8_t
    _pin;





};


#endif
