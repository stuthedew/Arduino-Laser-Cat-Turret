#ifndef _STUSERVO_H_
#define _STUSERVO_H_

#include <Servo.h>

struct servoPos{
  int
    min,
    max,
    current;

};

class StuServo: public Servo {

public:

  void
    setCalibration(int min, int max),
    calibrate(),
    stuWrite(int position);

  int
    getMin(),
    getMax();


private:

  servoPos
    _position;


};



#endif
