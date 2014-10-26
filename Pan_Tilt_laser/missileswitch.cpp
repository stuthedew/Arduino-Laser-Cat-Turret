
/**************************************************************************/
/*!
    @file     missileswitch.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for reading input from Sparkfun's missile switch
    (https://www.sparkfun.com/products/11310) and controlling its led.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/


#include "missileswitch.h"


Missileswitch::Missileswitch(int ledPin, int switchPin){

  _ledPin = ledPin;
  _switchPin = switchPin;

}

void Missileswitch::begin(){
  pinMode(_ledPin, OUTPUT);
  pinMode(_switchPin, INPUT);

  ledState(1);

}

void Missileswitch::ledState(bool state){
  _ledState = state;

  digitalWrite(_ledPin, _ledState);

}

bool Missileswitch::ledState(){

  return _ledState;
}
