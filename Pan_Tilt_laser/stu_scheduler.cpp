
/**************************************************************************/
/*!
    @file     stu_scheduler.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple scheduler to manage various events (e.g. sleep). Addapted from
    "Making Embedded Systems" by Elecia White.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/\

#include "stu_scheduler.h"

StuScheduler scheduler;


void Event::resetPeriodic(){
  _enabled = 1 ;
  time_t temp = _timeDelta + millis() + 5 ;
  setNextEventTime( temp );
  /*
  #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("Next Event: "));
    MY_SERIAL.println(temp);
    MY_SERIAL.println(F("Interval:"));
    MY_SERIAL.println(_timeDelta);
    MY_SERIAL.println();
  #endif
*/
}

void Event::setInterval(time_t mSec){

  _timeDelta = mSec;

}

time_t Event::getNextEventTime() const{
  return _endTime;

}

void Event::setNextEventTime(time_t mSec){
  //static time_t oldTime = 0;

  _endTime = mSec;
/*
  if( oldTime > _endTime ){
    #ifdef SERIAL_DEBUG
      MY_SERIAL.println(F("EVENT ROLLOVER!!!!"));
      MY_SERIAL.print(F("OLD TIME: "));
      MY_SERIAL.println(oldTime);
      MY_SERIAL.print(F("\tNEW TIME: "));
      MY_SERIAL.println(_endTime);
      delay(2000);
    #endif
    rolloverFlag ^= 1 ;
  }

  oldTime = _endTime ;
*/
}

void Event::disable( void ){
  _enabled = 0;

}


void Event::enable(){

  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("EVENT enabled"));
  #endif

  if( !_enabled ){
    resetPeriodic() ;
  }
}

bool Event::enabled() const {
  return _enabled;
}


Timer::Timer(time_t interval, bool enable){
  _timeDelta = interval ;
  _enabled = enable ;
  rolloverFlag = 0 ;

}

void Timer::start( void ){
  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("TIMER START ENABLE"));
  #endif
  enable();

}

void Timer::stop( void ){
  disable();

}

void Timer::restart( void ){
  #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("Restart"));
  #endif
  resetPeriodic();
}

void Timer::run( ){
  _elapsed = true ;
}

bool Timer::check( timer_input_e action ){
  if(!_enabled){
    return false;
  }

  if( _elapsed ){ //TRUE == elapsed

    #ifdef SERIAL_DEBUG
    MY_SERIAL.print(F("Timer Elapsed!!!!\nDuration: "));
    MY_SERIAL.println(_timeDelta);

    #endif

    _enabled = 0 ;
    _elapsed = 0;
    if(action == ELAPSE_RESTART){
      restart();
    }

    return true ;
  }
  return false ;
}


Task::Task( void (*cbFunc)(), time_t interval, bool enable):_callback(cbFunc) {
  _timeDelta = interval ;
  _enabled = enable ;
  rolloverFlag = 0 ;
}

Task::Task( time_t interval, bool enable) {
  rolloverFlag = 0 ;
  _timeDelta = interval ;
  _enabled = enable ;

}

void Task::changeCallback( void (*cbFunc)() ){
    _callback = cbFunc;
  }


void Task::run( void ){
    _enabled = 0 ;
    _callback();
  }

void StuScheduler::begin( void ){
  _tItr = 0 ;
  _milliRolloverFlag = 0 ;

}

void StuScheduler::addEvent( Event *t ){

  if(_tItr >= MAX_EVENTS){
    #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("TOO MANY EVENTS!!!"));
    #endif
    return;
  }

  _Event[ _tItr ] = t;
  _tItr++;

  #ifdef SERIAL_DEBUG
  MY_SERIAL.print(F("Event Total: "));
  MY_SERIAL.println( _tItr);

  #endif

}

void StuScheduler::restart( void ){
  #ifdef SERIAL_DEBUG
    MY_SERIAL.println(F("Schedule Restart"));
  #endif

  for(uint8_t i = 0; i < _tItr; i++){
    if( _Event[i]->enabled() ){
      _Event[i]->resetPeriodic();
    }
  }
}

void StuScheduler::run( void ){
  static time_t oldTime;
  time_t currentTime = millis();

  // HANDLE ROLLOVER
  if( oldTime > currentTime ){ //rollover
    #ifdef SERIAL_DEBUG
    MY_SERIAL.println( F("MILLI ROLLOVER!!!!") );
    #endif
    _milliRolloverFlag ^= 1; // Toggle rollover flag
  }

  for(uint8_t i = 0; i < _tItr; i++){

    if( _Event[i]->enabled() ){

      if( _Event[i]->getNextEventTime() <= currentTime && _milliRolloverFlag == _Event[i]->rolloverFlag){

        _Event[i]->run();
      }
    }
  }
    oldTime = currentTime;
}
