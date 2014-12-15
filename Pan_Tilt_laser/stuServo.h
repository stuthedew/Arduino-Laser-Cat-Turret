
/**************************************************************************/
/*!
    @file     stuServo.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v0.0.1 - First release
    v0.0.2 - Added power control (MOSFET)

*/
/**************************************************************************/


#pragma once

#include "Arduino.h"
#include <Servo.h>


#define MG995_MIN_MICRO 800
#define MG995_MAX_MICRO 2400

struct servoPos{

    int
      min ,
      max ,
      current ;
};

class StuServo: public Servo {

public:

    void
      begin( void ) ,
      setPowerPin( uint8_t pwrPin ) ,
      setCalibration( int min, int max ) ,
      stuWrite( int position ),
      pause( void ),
      wake( void ) ;


    int
      getMin( void ) const ,
      getMax( void ) const ;

private:

    servoPos
      _position ;

    uint8_t
      _powerPin ;

};
