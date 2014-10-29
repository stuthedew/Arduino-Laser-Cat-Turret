
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



#include "stu_vector.h"


SVector::SVector(int x, int y){
  _x = x;
  _y = y;
}

int SVector::x(){

  return _x;
}

int SVector::y(){

  return _y;
}

SVector SVector::add(int x, int y){
  SVector rV(_x+x, _y+y);

  return rV;
}

SVector SVector::add(SVector *v){
  SVector rV(_x + v->x(), _y + v->y());

  return rV;
}

SVector SVector::sub(int x, int y){
  SVector rV(_x - x, _y - y);

  return rV;
}

SVector SVector::sub(SVector *v){
  SVector rV(_x - v->x(), _y - v->y());

  return rV;
}

SVector SVector::set(int x, int y){
  SVector rV(x, y);

  return rV;
}

SVector SVector::set(SVector *v){
  SVector rV(v->x(), v->y());

  return rV;
}

float SVector::distance(int x, int y){

  return sqrt(pow((x - _x), 2) + pow((y - _y),2));
}

float SVector::distance(SVector *v){

  return sqrt(pow((v->x() - _x), 2) + pow((v->y() - _y),2));
}

float SVector::getMag(){

  return sqrt(_x*_x + _y*_y);
}
