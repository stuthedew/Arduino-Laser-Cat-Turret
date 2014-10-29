
/**************************************************************************/
/*!
    @file     stu_vector.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for working with vectors. I got the idea from the
    awesome book The Nature of Code: Simulating Natural Systems with
    Processing by Daniel Shiffman http://natureofcode.com. You can
    download it for free, but it's definitely worth buying!
    (http://www.amazon.com/gp/product/0985930802)


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STU_VECTOR_H_
#define _STU_VECTOR_H_

#if(ARDUINO > 100)
#include "Arduino.h"
#include <math.h>
#endif

class SVector {

public:

  SVector(int x, int y);

  SVector
    add(int x, int y),
    add(SVector *),
    sub(int x, int y),
    sub(SVector *),
    set(int x, int y),
    set(SVector *),
    mult(int),
    div(float),
    rotate(float theta),
    setMag(float);


  int
    x(),
    y(),
    dot(int x, int y),
    dot(SVector *);

  float
    distance(int x, int y),
    distance(SVector *),
    mag(),
    heading(),
    angleBetween(SVector *),
    angleBetween(int x, int y);


private:
  int
    _x,
    _y;

};



#endif
