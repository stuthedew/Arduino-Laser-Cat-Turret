
/**************************************************************************/
/*!
    @file     stuLaser.cpp
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


#include "stuLaser.h"

StuLaser::StuLaser(uint8_t pin){
  _pin = pin;

}


void StuLaser::begin(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
}

void StuLaser::fire(boolean state){
digitalWrite(_pin, state);

}
