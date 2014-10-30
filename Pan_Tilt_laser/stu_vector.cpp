
/**************************************************************************/
/*!
    @file     stu_vector.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for working with vectors based on processing
    (processing.org). I got the idea from the awesome book: "The Nature of
    Code: Simulating Natural Systems with Processing" by Daniel Shiffman
    (http://natureofcode.com). You can download it for free from his
    website, but it's definitely worth buying!
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

SVector SVector::mult(int val){
  SVector rV(_x * val, _y * val);

  return rV;
}

SVector SVector::div(float val){
  SVector rV(round(_x / val), round(_y / val));

  return rV;
}

float SVector::distance(int x, int y){

  return sqrt(pow((x - _x), 2) + pow((y - _y),2));
}

float SVector::distance(SVector *v){

  return distance(v->x(), v->y());
}

float SVector::mag(){

  return sqrt(_x*_x + _y*_y);
}

float SVector::magSq(){

  return (_x*_x + _y*_y);
}

float SVector::dot(int x, int y){
  return(_x*x + _y*y);
}

float SVector::dot(SVector *v){
  return dot(v->x(), v->y());
}

float SVector::heading(){
  return -1*atan2(-1*_y, _x);
}

SVector SVector::setMag(float newMag){
  SVector  rV = mult(newMag);
  rV = rV.div(mag());

  return rV;
}

SVector SVector::fromAngle(float theta){
  SVector rV(round(cos(theta)*100), round(sin(theta)*100));
  return rV;
}

float SVector::angleBetween(SVector *v){
  // We get NaN if we pass in a zero vector which can cause problems
  // Zero seems like a reasonable angle between a (0,0) vector and something else
  if (_x == 0 && _y == 0) return 0;
  if (v->x() == 0 && v->y() == 0) return 0;

  // This should be a number between -1 and 1, since it's "normalized"
  float amt = dot(v) / (mag() * v->mag());
  Serial.println(acos(amt)*180 / M_PI);
  // But if it's not, due to rounding error, then we need to fix it
  // http://code.google.com/p/processing/issues/detail?id=340
  // Otherwise if outside the range, acos() will return NaN
  // http://www.cppreference.com/wiki/c/math/acos
  if (amt <= -1) {
    return M_PI;
  } else if (amt >= 1) {

    return 0;
  }
  return acos(amt);
}

float SVector::angleBetween(int x, int y){
  SVector rV(x, y);
  return angleBetween(&rV);
}

SVector SVector::rotate(float theta){
  int temp = _x;
  SVector rV(round(_x * cos(theta) - _y*sin(theta)), round(temp*sin(theta) + _y * cos(theta)));

  return rV;
}
