
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

void Event::initialize( void ){
  rolloverFlag = 0 ;
}

void Event::resetPeriodic(){
  _enabled = 1 ;
  setNextEventTime( _timeDelta + _endTime );

}

void Event::setInterval(time_t mSec){

  _timeDelta = mSec;
  setNextEventTime( _timeDelta ) ;

}

time_t Event::getNextEventTime() const{
  return _endTime;

}

void Event::setNextEventTime(time_t mSec){
  static time_t oldTime;

  _endTime = mSec;

  if( oldTime > _endTime ){
    rolloverFlag ^= 1 ;
  }

  oldTime = _endTime ;

}

void Event::disable( void ){
  _enabled = 0;

}


void Event::enable(){
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

}

void Timer::start( void ){
  enable();

}

void Timer::stop( void ){
  disable();

}

void Timer::restart( void ){
  stop();
  start();
}


bool Timer::check( timer_input_e action ){
  if(!_enabled){
    return false;
  }

  if( _endTime < millis() ){ //TRUE == elapsed
    _enabled = 0 ;
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
}

void Task::changeCallback( void (*cbFunc)() ){
    _callback = cbFunc;
  }


void Task::run( void ){
    _enabled = 0 ;
    _callback();
  }

void StuScheduler::initialize( void ){
  _tItr = 0 ;
  _milliRolloverFlag = 0 ;

}

void StuScheduler::addTask( Task *t ){
  _Task[ _tItr ] = t;
  _tItr++;

}

void StuScheduler::restart( void ){
  for(uint8_t i = 0; i < _tItr; i++){
    if( _Task[i]->enabled() ){
      _Task[i]->resetPeriodic();
    }
  }
}

void StuScheduler::run( void ){
  static time_t oldTime;

  time_t currentTime = millis();

  // HANDLE ROLLOVER
  if( oldTime > currentTime ){ //rollover

    _milliRolloverFlag ^= 1; // Toggle rollover flag
  }

  for(uint8_t i = 0; i < _tItr; i++){

    if( _Task[i]->enabled() ){

      if( _Task[i]->getNextEventTime() <= currentTime && _milliRolloverFlag == _Task[i]->rolloverFlag){

        _Task[i]->run();
        _Task[i]->resetPeriodic();

      }
    }
  }
    oldTime = currentTime;
}
