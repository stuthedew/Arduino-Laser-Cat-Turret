
/**************************************************************************/
/*!
    @file     missileswitch.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for reading input from Sparkfun's missile switch
    (https://www.sparkfun.com/products/11310) and controlling its led.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#pragma once

#include "Arduino.h"

namespace stu {

  class Missileswitch {
    public:

      Missileswitch(int, int);

      void
        begin(),
        ledState(bool),
        heartBeat(int);

      bool
        ledState() const,
        switchState() ;


    private:
      const int
        _ledPin,
        _switchPin;

      bool
        _ledState,
        _switchState,
        _debounce();

  };
}
