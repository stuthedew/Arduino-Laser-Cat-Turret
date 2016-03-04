
/**************************************************************************/

/*!
    @file     stuServo.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Augmented servo library based off of Arduino Servo library. Adds maximum
    and minimum angles, and prevents servo from going past them.


    @section  HISTORY
    v1.0 - First release

 */

/**************************************************************************/


#ifndef _STUSERVO_H_
#define _STUSERVO_H_

#include <Servo.h>

struct servoPos {
  int
    min,
    max,
    current;
};

class StuServo : public Servo {
public:

  void
  setCalibration(int min,
                 int max),
  calibrate(),
  stuWrite(int position);

  int
  getMin(),
  getMax();

private:

  servoPos
    _position;
};


#endif /* ifndef _STUSERVO_H_ */
