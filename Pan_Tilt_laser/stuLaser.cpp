#include "stuLaser.h"

StuLaser::StuLaser(uint8_t pin){
  _pin = pin;

}


void StuLaser::begin(){
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
}

void StuLaser::fire(boolean state){
digitalWrite(_pin, state);

}
