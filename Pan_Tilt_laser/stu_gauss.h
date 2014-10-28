
/**************************************************************************/
/*!
    @file     stu_gauss.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Wrapper library for Gaussian library
    (https://github.com/ivanseidel/Gaussian) to create random number
    generator with custom distribution.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STU_GAUSS_H_
#define _STU_GAUSS_H_

#include "Arduino.h"
#include <Gaussian.h>

class StuGauss {

public:

  StuGauss();


  unsigned long
    gRandom(unsigned long zero, double variance);



private:

  Gaussian _gauss;


};

#endif
