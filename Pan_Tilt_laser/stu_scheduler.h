
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

namespace stu {

typedef void (*Callback)(void);

class Task{

public:

  Task( void (*callback)(), unsigned long interval=100, bool enable=1 ) ;

            void    resetPeriodic( void ) ;
            void    setInterval( unsigned long mSec ) ;
            void    setNextRunTime( unsigned long mSec ) ;
            void    disable() ;
            void    enable() ;
            void    run() ;
            void    changeCallback( void ( *callback )( void ) ) ;

            bool    enabled( void ) const ;

  unsigned  long    nextRunTime( void ) const ;

private:
            bool    _enabled ;

        Callback    _callback ;

  unsigned long     _runNextAt ;
  unsigned long     _timeBetweenRuns ;

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


}
