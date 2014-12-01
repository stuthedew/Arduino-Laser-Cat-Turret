
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


typedef void (*Callback)(void);

//action on elpased timer
typedef enum{

  ELAPSE_RESTART,  // Restart if timer elapsed
  ELAPSE_DISABLE   // Disable if timer elapsed

}timer_input_e;

class Event{

public:

            void    resetPeriodic( void ) ;
            void    setInterval( unsigned long mSec ) ;
            void    setNextEventTime( unsigned long mSec ) ;
            void    disable( void ) ;
            void    enable( void ) ;
   virtual  void    run( void ) ;

            bool    enabled( void ) const ;

  unsigned  long    getNextEventTime( void ) const ;

protected:
            bool    _enabled ;

  unsigned long     _endTime ;
  unsigned long     _timeDelta ;

};

class Timer: public Event{

public:

    Timer(unsigned long interval=0, bool enable=0) ;

    void start( void ) ;
    void stop( void ) ;
    void restart( void ) ;
    bool check( timer_input_e action = ELAPSE_DISABLE ) ;


};

class Task: public Event{

public:

  Task( void (*callback)(), unsigned long interval=100, bool enable=0 ) ;

  void    run( void ) ;
  void    changeCallback( void ( *callback )( void ) ) ;


  unsigned  long    nextRunTime( void ) const ;

private:

  Callback    _callback ;

};



class StuScheduler {

public:
  StuScheduler( void );


            void    addTask( Task *t ) ;
            void    run( void ) ;
            void    restart( void ) ;


private:

            Task    *_Task[ 20 ] ;

          uint8_t   _tItr ;

};
