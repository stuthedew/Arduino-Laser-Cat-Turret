
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
static settings_t int_state( 1, LED_ON, LED_ON, LED_OFF, STATE_RUN );
static settings_t sleep_state( 1, LED_ON, LED_OFF, LED_ON, STATE_RUN );
static settings_t off_state( 0, LED_OFF, LED_OFF, LED_OFF, STATE_OFF);
static settings_t rest_state( 0, LED_BLINK, LED_ON, LED_OFF, STATE_REST);

PanTilt::PanTilt(uint8_t xPin, uint8_t yPin ):_xServo(), _yServo(),
  _display( POWER_PIN,  CONT_PIN, INT_PIN ),
  posX( SERVO_MIN_X_AXIS, SERVO_MAX_X_AXIS, -LASER_MIDPOINT_OFFSET_X-7, LASER_PROBABILITY_X ),
  posY( SERVO_MIN_Y_AXIS, SERVO_MAX_Y_AXIS, -LASER_MIDPOINT_OFFSET_Y, LASER_PROBABILITY_Y),
  _laser(LASER_PIN), _offMode( &off_state ), _contMode( &on_state ), _intMode( &int_state, INTERMITTENT_ON_TIME, &rest_state, INTERMITTENT_OFF_TIME ), _sleepMode(  &sleep_state, MINUTES_BEFORE_SLEEP, &off_state ), _stateChangeTask(), _currentState(&_currentMode->currentSettings->state->id) {


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
  PanTilt::_setMode(MODE_OFF);
  scheduler.begin() ;
  _laser.begin();
  _xServo.attach(_xPin) ;
  _yServo.attach(_yPin) ;
  _xServo.setPowerPin( X_PWR_PIN ) ;
  _yServo.setPowerPin( Y_PWR_PIN ) ;

  _dial.setPin( DIAL_PIN );
  _dial.begin() ;

  _display.begin() ;
  scheduler.addEvent(&_stateChangeTask);

  posX.angle = posX.minAngle;
  posY.angle = posY.minAngle;
  _updateAngles();
  delay(1200);

  posX.angle = posX.maxAngle;
  posY.angle = posY.maxAngle;
  _updateAngles();
  delay(1300);

  posX.angle = posX.midAngle;
  posY.angle = posY.midAngle;
  _updateAngles();
  delay(450);
  _laser.fire(1);

  delay(2000);
  _laser.fire(0);
  PanTilt::_setMode(MODE_OFF);


}

void PanTilt::_setMode( runmode_e mode ){

  _stateChangeTask.disable();

  switch(mode){

    case MODE_OFF:
      _laser.fire(0);
      posX.angle = posX.midAngle;
      posY.angle = posY.midAngle;
      _updateAngles();
      delay(250);
      _setState(&off_state);
      _currentMode = &_offMode;

      #ifdef SERIAL_DEBUG
        MY_SERIAL.println(F("MODE set to OFF"));
      #endif
      break;

    case MODE_CONTINUOUS:

      _setState(&on_state);
      _currentMode = &_contMode;


      #ifdef SERIAL_DEBUG
        MY_SERIAL.println(F("MODE set to CONTINUOUS"));
      #endif

      break;

    case MODE_INTERMITTENT:
      _setState(&int_state);
      _currentMode = &_intMode;

      #ifdef SERIAL_DEBUG
      MY_SERIAL.println(F("MODE set to INTERMITTENT"));
      #endif

      break;

    case MODE_SLEEP:

      _currentMode = &_sleepMode;
      _setState(&sleep_state);

      #ifdef SERIAL_DEBUG
      MY_SERIAL.println(F("MODE set to SLEEP"));
      #endif

      break;
  }

  _currentMode->currentSettings = &_currentMode->settingA;
  _currentMode->nextSettings = &_currentMode->settingB;

  _setState( _currentMode->currentSettings->state );

  if( _currentMode->currentSettings->duration > 0){
    _stateChangeTask.setInterval( _currentMode->currentSettings->duration );
    _stateChangeTask.enable();
    #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("PanTilt setmode Enable\n"));
    #endif
}

  _display.update();

  _mode = mode;
}


Task* PanTilt::getTaskPtr( void ){
  return &_stateChangeTask;

}


Callback PanTilt::callback( void ){

  _stateChangeTask.disable();


  #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("STATE CALLBACK"));
  #endif


  statePair_t* tmp;
  tmp = _currentMode->currentSettings;
  _currentMode->currentSettings = _currentMode->nextSettings;

  _setState( _currentMode->currentSettings->state );

  if( _currentMode->currentSettings->duration > 0){
    _currentMode->nextSettings = tmp;

    _stateChangeTask.setInterval( _currentMode->currentSettings->duration );

    #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("PanTilt callback Enable\n"));
    #endif
    _stateChangeTask.enable();
    delay(25);
  }
}


void PanTilt::_setState( settings_t* s ){

  _display.setLEDStates( s->ledState[0], s->ledState[1], s->ledState[2] );
  _laser.fire(s->laserState);

  if(s->servoState){
    _xServo.wake();
    _yServo.wake();
  }else{
    _xServo.pause();
    _yServo.pause();
  }

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

void PanTilt::detach( void ){
  _xServo.detach();
  _yServo.detach();
}


void PanTilt::update( void ){
  runmode_e mode = _dial.getMode();
  if(mode != _mode){ //don't change mode if the same as current.
    PanTilt::_setMode( mode ) ;
  }


  _display.update();
  if( getState() == STATE_RUN){
    _updateAngles();
  }
}

void PanTilt::_updateAngles( void ){

  _xServo.stuWrite(posX.angle);
  _yServo.stuWrite(posY.angle);

}

void PanTilt::pause( unsigned long pauseVal, bool laserState ){
  _laser.fire(laserState);
  _xServo.pause();
  _yServo.pause();
  delay( pauseVal );
  _xServo.wake();
  _yServo.wake();
  _laser.fire(1);

}

void PanTilt::shake( void ){
  int moveVal = 20;
  const int shakeDelay = 0;
  posX.angle += moveVal;
  PanTilt::update();
  delay(shakeDelay);
  posX.angle -= 2*moveVal;
  PanTilt::update();
  delay(shakeDelay);
  posX.angle += moveVal;
  PanTilt::update();
  delay(shakeDelay);
}
