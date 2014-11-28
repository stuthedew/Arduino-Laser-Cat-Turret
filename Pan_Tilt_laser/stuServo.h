
/**************************************************************************/
/*!
    @file     stuServo.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/


#pragma once

#include <Servo.h>

namespace stu{

struct servoPos{

        int       min ;
        int       max ;
        int       current ;
};

class StuServo: public Servo {

public:

          void    setCalibration( int min, int max ) ;
          void    calibrate( void ) ;
          void    stuWrite( int position ) ;

          int     getMin( void ) const ;
          int     getMax( void ) const ;

private:

      servoPos    _position ;

};
}
