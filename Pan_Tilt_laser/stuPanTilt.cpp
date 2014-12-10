
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


static settings_t on_state( 1, LED_ON, LED_OFF, LED_OFF, STATE_RUN );
static settings_t off_state( 0, LED_OFF, LED_OFF, LED_OFF, STATE_OFF);
static settings_t rest_state( 0, LED_OFF, LED_BLINK, LED_OFF, STATE_REST);

PanTilt::PanTilt(uint8_t xPin, uint8_t yPin ):_xServo(), _yServo(),
  _display( POWER_PIN,  CONT_PIN, INT_PIN ), posX( 50, 120, -30 ), posY( 7, 45, 0, 10),
  _laser(LASER_PIN), _offMode( &off_state ), _contMode( &on_state ), _intMode( &on_state, INTERMITTENT_RUN_TIME, &rest_state, INTERMITTENT_REST_TIME ), _sleepMode(  &on_state, TIME_BEFORE_SLEEP, &off_state ), _stateChangeTimer() {

  _modes[ 0 ] = &_offMode;
  _modes[ 1 ] = &_contMode;
  _modes[ 2 ] = &_intMode;
  _modes[ 3 ] = &_sleepMode;

  _xPin = xPin;
  _yPin = yPin;

  _xServo.setCalibration(posX.minAngle, posX.maxAngle);
  _yServo.setCalibration(posY.minAngle, posY.maxAngle);

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
  scheduler.addEvent(&_stateChangeTimer);

  posX.angle = posX.minAngle;
  posY.angle = posY.minAngle;
  update();
  delay(500);

  posX.angle = posX.maxAngle;
  posY.angle = posY.maxAngle;
  update();
  delay(500);

  _laser.fire(1);
  posX.angle = posX.midAngle;
  posY.angle = posY.midAngle;
  update();

  delay(500);
  _laser.fire(0);

}

void PanTilt::setMode( runmode_e mode ){

  _stateChangeTimer.disable();

  switch(mode){

    case MODE_OFF:

      _setState(&off_state);
      _currentMode = &_offMode;

      #ifndef EMBED
        Serial.println(F("MODE set to OFF"));
      #endif

      break;

    case MODE_CONTINUOUS:

      _setState(&on_state);
      _currentMode = &_contMode;

      #ifndef EMBED
        Serial.println(F("MODE set to CONTINUOUS"));
      #endif

      break;

    case MODE_INTERMITTENT:
      _setState(&on_state);
      _display.setLEDState( 1, LED_ON ); //set INTERMITTENT led to ON
      _currentMode = &_intMode;

      #ifndef EMBED
      Serial.println(F("MODE set to INTERMITTENT"));
      #endif

      break;

    case MODE_SLEEP:
      _setState(&on_state);
      _currentMode = &_sleepMode;
      _display.setLEDState( 2, LED_BLINK ); //set SLEEP led to BLINK

      #ifndef EMBED
      Serial.println(F("MODE set to SLEEP"));
      #endif

      break;
  }

  if( _currentMode->currentSettings->duration > 0){
    _stateChangeTimer.setInterval( _currentMode->currentSettings->duration );
    _stateChangeTimer.enable();
}

  _display.update();
}


Task* PanTilt::getTaskPtr( void ){
  return &_stateChangeTimer;

}


Callback PanTilt::callback( void ){

  _stateChangeTimer.disable();

  if( _currentMode->nextSettings != NULL){
    #ifndef EMBED
      Serial.println(F("STATE CALLBACK"));
    #endif
    statePair_t* tmp;
    tmp = _currentMode->currentSettings;
    _currentMode->currentSettings = _currentMode->nextSettings;

    _setState( _currentMode->currentSettings->state );

    _currentMode->nextSettings = tmp;

    _stateChangeTimer.setInterval( _currentMode->currentSettings->duration );
    _stateChangeTimer.enable();

  }
}


void PanTilt::_setState( settings_t* s ){

  _display.setLEDStates( s->ledState[0], s->ledState[1], s->ledState[2] );
  _laser.fire(s->laserState);

}

state_e PanTilt::getState( void ) const{
  return _currentMode->currentSettings->state->id;
}


void PanTilt::setPosition( int X, int Y ){
  posX.angle = 90;
  posY.angle = 90;

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

  _xServo.stuWrite(posX.angle);
  delay(10);
  _yServo.stuWrite(posY.angle);
  delay(abs((posX.angle - oldXangle)+15));
  delay(abs((posY.angle - oldYangle)+15));

  oldXangle = posX.angle;
  oldYangle = posY.angle;

}

void PanTilt::shake( void ){
  int moveVal = 10;
  const int shakeDelay = 0;
  posX.angle += moveVal;
  update();
  delay(shakeDelay);
  posX.angle -= 2*moveVal;
  update();
  delay(shakeDelay);
  posX.angle += moveVal;
  update();
  delay(shakeDelay);
}
