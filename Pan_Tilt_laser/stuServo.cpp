#include "stuServo.h"


void StuServo::calibrate(){


}

void StuServo::setCalibration(int min, int max){
  _position.min = min;
  _position.max = max;


}


void StuServo::stuWrite(int position){

  if(position < _position.min){
    write(_position.min);
  }
  else if(position > _position.max){
    write(_position.max);
  }
  else{
    write(position);
  }

}

int StuServo::getMin(){
  return _position.min;
}

int StuServo::getMax(){
  return _position.max;
}
