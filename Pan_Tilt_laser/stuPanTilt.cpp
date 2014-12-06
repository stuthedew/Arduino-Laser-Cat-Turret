
/**************************************************************************/
/*!
    @file     stuPanTilt.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for controlling dual servo pan-tilt rig
    (https://www.adafruit.com/products/1967) with an Arduino Uno (Atmel
    Atmega 328).


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#include "stuPanTilt.h"



PanTilt::PanTilt(uint8_t xPin, uint8_t yPin ):_xServo(), _yServo(), _display( POWER_PIN, CONT_PIN, INT_PIN ), _posX( 50, 120, -30 ), _posY( 7, 45, 0, 10), _laser(LASER_PIN) {

  _xPin = xPin;
  _yPin = yPin;

  _xServo.setCalibration(_posX.minAngle, _posX.maxAngle);
  _yServo.setCalibration(_posY.minAngle, _posY.maxAngle);

  _modes[ 0 ] = &_offMode ;
  _modes[ 1 ] = &_contMode ;
  _modes[ 2 ] = &_intMode ;
  _modes[ 3 ] = &_restMode ;
  _modes[ 4 ] = &_sleepMode ;

}

void PanTilt::begin( void ){
  scheduler.initialize() ;
  _laser.begin();
  delay( 500 ) ;
  _xServo.attach(_xPin) ;
  _yServo.attach(_yPin) ;
  _xServo.setPowerPin( X_PWR_PIN ) ;
  _yServo.setPowerPin( Y_PWR_PIN ) ;

  _dial.setPin( DIAL_PIN );
  _dial.begin() ;

  _display.begin() ;


  _posX.angle = _posX.minAngle;
  _posY.angle = _posY.minAngle;
  update();
  delay(500);

  _posX.angle = _posX.maxAngle;
  _posY.angle = _posY.maxAngle;
  update();
  delay(500);

  _laser.fire(1);
  _posX.angle = _posX.midAngle;
  _posY.angle = _posY.midAngle;
  update();

  delay(500);
  _laser.fire(0);

}

void PanTilt::setMode( runmode_e mode ){
    static runmode_e oldDialMode;
    if( oldDialMode == mode ){ //return if nothing has changed
      return ;
    }
    _mode = mode ;
    oldDialMode = _mode ;
    _display.setMode( _mode ) ;

}

void PanTilt::setPosition( int X, int Y ){
  _posX.angle = 90;
  _posY.angle = 90;

}

runmode_e PanTilt::getMode( void ) const{

  return _mode ;
}

void PanTilt::detach(){
  _xServo.detach();
  _yServo.detach();
}

void PanTilt::update(){
  _dial.update() ;
  setMode( _dial.getMode() ) ;
  scheduler.run();
  _display.update();
  _updateAngles();
}

void PanTilt::_updateAngles(){
  static int oldXangle, oldYangle ;

  _xServo.stuWrite(_posX.angle);
  delay(10);
  _yServo.stuWrite(_posY.angle);
  delay(abs((_posX.angle - oldXangle)+15));
  delay(abs((_posY.angle - oldYangle)+15));

  oldXangle = _posX.angle;
  oldYangle = _posY.angle;

}

void PanTilt::shake( void ){
  int moveVal = 10;
  const int shakeDelay = 0;
  _posX.angle += moveVal;
  update();
  delay(shakeDelay);
  _posX.angle -= 2*moveVal;
  update();
  delay(shakeDelay);
  _posX.angle += moveVal;
  update();
  delay(shakeDelay);
}
