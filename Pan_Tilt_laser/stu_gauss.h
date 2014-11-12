
/**************************************************************************/
/*!
    @file     stu_gauss.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Wrapper library for Gaussian library
    (https://github.com/ivanseidel/Gaussian) to create random number
    generator with custom distribution.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/
#pragma once

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
