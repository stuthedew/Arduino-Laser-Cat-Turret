
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



  void Event::resetPeriodic(){

    _endTime = _timeDelta + millis();
  }

  void Event::setInterval(unsigned long mSec){
    _timeDelta = mSec;
    _endTime = _timeDelta + millis();

  }

  unsigned long Event::getNextEventTime() const{
    return _endTime;

  }

  void Event::setNextEventTime(unsigned long mSec){
    _endTime = mSec;

  }



  void Event::disable( void ){
    _enabled = 0;

  }

  void Event::run( void ){
    return 0;
  }


  void Event::enable(){
    if( !_enabled ){
      _enabled = 1 ;
      resetPeriodic() ;
  }
  }



  bool Event::enabled() const {
    return _enabled;
  }


  Timer::Timer(unsigned long interval, bool enable):_timeDelta(interval),_enabled(enable) {

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
    if(action == ELAPSE_RESTART){
      restart();
    }
    return true ;
  }
  return false ;
}


  Task::Task( void (*cbFunc)(), unsigned long interval, bool enable):_callback(cbFunc),_timeDelta(interval),_enabled(enable) {

  }

  void Task::changeCallback( void (*cbFunc)() ){
    _callback = cbFunc;
  }


  void Task::run( void ){
    _callback();
  }

  StuScheduler::StuScheduler( void ){
    _tItr = 0;
  }

  void StuScheduler::addTask( Task *t ){
  _Task[ _tItr ] = t;
  _tItr++;

  }

  void StuScheduler::restart( void ){
    for(uint8_t i = 0; i < _tItr; i++){
      if(_Task[i]->enabled()){
        _Task[i]->resetPeriodic();
      }
    }
  }

  void StuScheduler::run( void ){
    for(uint8_t i = 0; i < _tItr; i++){
      if(_Task[i]->enabled() && _Task[i]->nextRunTime() <= millis()){
        _Task[i]->run();
        _Task[i]->resetPeriodic();
      }
    }
  }
