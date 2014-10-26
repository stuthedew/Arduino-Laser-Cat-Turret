
/**************************************************************************/
/*!
    @file     missileswitch.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for reading input from Sparkfun's missile switch
    (https://www.sparkfun.com/products/11310) and controlling its led.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _MISSILESWITCH_H_
#define _MISSILESWITCH_H_

#include "Arduino.h"


class Missileswitch {
  public:

    Missileswitch(int, int);

    void
      begin(),
      ledState(bool),
      heartBeat(int);

    bool
      ledState(),
      switchState();


  private:
    const int
      _ledPin,
      _switchPin;

    bool
      _ledState,
      _switchState,
      _debounce();



};





#endif
