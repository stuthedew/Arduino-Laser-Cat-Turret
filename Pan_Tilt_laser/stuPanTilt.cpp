#include "stuPanTilt.h"



PanTilt::PanTilt(uint8_t xPin, panTiltPos_t *xPos, uint8_t yPin, panTiltPos_t *yPos, int height):_xServo(), _yServo(), _height(height){

  _xPin = xPin;
  _yPin = yPin;

  _Xpos = xPos;
  _Ypos = yPos;

  _xServo.setCalibration(_Xpos->minAngle, _Xpos->maxAngle);
  _yServo.setCalibration(_Ypos->minAngle, _Ypos->maxAngle);


}

void PanTilt::begin(){
  _xServo.attach(_xPin);
  _yServo.attach(_yPin);
  delay(500);
}

void PanTilt::detach(){
  _xServo.detach();
  _yServo.detach();
}

void PanTilt::_setAnglesFromPosition(){
_Xpos->angle = _rad2Deg(atan(_Ypos->pos/_Xpos->pos));
_Ypos->angle = _rad2Deg(atan(sqrt(square(_Ypos->pos) + square(_Xpos->pos)) /_height));
}

void PanTilt::_setPositionFromAngles(){

float adjacent = _height / tan(_deg2Rad(90-_Ypos->angle));
_Ypos->pos = sin(_deg2Rad(_Xpos->angle)) * adjacent;
_Xpos->pos = cos(_deg2Rad(_Xpos->angle)) * adjacent;
}
/*
void PanTilt::updatePosition(){
  _setAnglesFromPosition();
  bool overRunFlag = 0;
  if(_Ypos->angle >= _Ypos->maxAngle - 1){
    _Ypos->angle = _Ypos->maxAngle;

    overRunFlag = 1;
  }
  else if(_Ypos->angle <= _Ypos->minAngle + 1){
    _Ypos->angle = _Ypos->minAngle;

    overRunFlag = 1;

  }

  if(_Xpos->angle >= _Xpos->maxAngle - 1){
    _Xpos->angle = _Xpos->maxAngle;


    overRunFlag = 1;
  }
  else if(_Xpos->angle <= _Xpos->minAngle +1){

    _Xpos->angle = _Xpos->minAngle;

    overRunFlag = 1;
  }
  if(overRunFlag){
    Serial.println(F("########OVERRUN#########"));
    updateAngles();
    return;
  }
  _update();

}
*/
void PanTilt::updateAngles(){
  //_setPositionFromAngles();
  _update();

}

void PanTilt::_update(){
static int oldXangle, oldYangle;

  Serial.print(F("X angle: "));
  Serial.println(_Xpos->angle);
  Serial.print(F("Y angle: "));
  Serial.println(_Ypos->angle);
  /*
  Serial.print(F("X pos: "));
  Serial.println(_Xpos->pos);
  Serial.print(F("Y pos: "));
  Serial.println(_Ypos->pos);
  Serial.println();
*/
  _xServo.stuWrite(_Xpos->angle);

  _yServo.stuWrite(_Ypos->angle);
  delay(abs(_Xpos->angle - oldXangle)+15);
  delay(abs(_Ypos->angle - oldYangle)+15);

  oldXangle = _Xpos->angle;
  oldYangle = _Ypos->angle;

}

float PanTilt::_rad2Deg(float radVal){
#define RAD2DEG_CONVERSION_FACTOR 57.2958

  return radVal * RAD2DEG_CONVERSION_FACTOR;
}

float PanTilt::_deg2Rad(float degVal){
#define DEG2RAD_CONVERSION_FACTOR 0.01745

  return degVal * DEG2RAD_CONVERSION_FACTOR;
}
