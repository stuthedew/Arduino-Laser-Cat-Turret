
/**************************************************************************/
/*!
    @file     stuServo.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v0.0.1 - First release
    v0.0.2 - Switched write to writeMicroseconds to allow maximal servo
             rotation.
    v0.0.3 - Added 5 microsecond delay after wake to allow capacitors to charge.
    v0.0.4 - Added a readMicroseconds function to get position.

*/
/**************************************************************************/


#pragma once

#include "Arduino.h"
#include <Servo.h>
#include "Arduino.h"

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

    int
      _microSeconds ;

};
