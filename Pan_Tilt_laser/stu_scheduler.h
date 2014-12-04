
/**************************************************************************/
/*!
    @file     stu_scheduler.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple scheduler to manage various events (e.g. sleep). Addapted from
    "Making Embedded Systems" by Elecia White.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/
#pragma once

#include "Arduino.h"
#include <Time.h>

typedef void (*Callback)(void);

//action on elpased timer
typedef enum{

  ELAPSE_RESTART,  // Restart if timer elapsed
  ELAPSE_DISABLE   // Disable if timer elapsed

}timer_input_e;

class Event{

public:

    void
      resetPeriodic( void ) ,
      setInterval( time_t mSec ) ,
      setNextEventTime( time_t mSec ) ,
      disable( void ) ,
      enable( void ) ;

    virtual void
      run( void ) ;

    bool
      enabled( void ) const ;

    time_t
      getNextEventTime( void ) const ;


protected:
    bool
      _enabled ;

    time_t
      _endTime ,
      _timeDelta ;

};

class Timer: public Event{

public:

    Timer(time_t interval=0, bool enable=0) ;

    void
      start( void ) ,
      stop( void ) ,
      restart( void ) ;

    bool
      check( timer_input_e action = ELAPSE_DISABLE ) ;


};

class Task: public Event{

public:

    Task( void (*callback)(), time_t interval=100, bool enable=0 ) ;

    void
      run( void ) ,
      changeCallback( void ( *callback )( void ) ) ;


    time_t
      nextRunTime( void ) const ;

private:

    Callback
      _callback ;

};

class StuScheduler {

public:

    StuScheduler( void );

    void
      addTask( Task *t ) ,
      run( void ) ,
      restart( void ) ;


private:

    Task
      *_Task[ 20 ] ;

    uint8_t
      _tItr ;



};

extern StuScheduler scheduler;
