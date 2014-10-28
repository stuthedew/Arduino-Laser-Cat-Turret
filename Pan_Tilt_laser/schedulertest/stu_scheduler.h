
/**************************************************************************/
/*!
    @file     stu_scheduler.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple scheduler to manage various events (e.g. sleep). Addapted from
    "Making Embedded Systems" by Elecia White.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STU_SCHEDULER_H_
#define _STU_SCHEDULER_H_

#include "Arduino.h"


typedef void (*Callback)(void);

class Task{
public:

  Task(void (*cbFunc)());

  void
    resetPeriodic(),
    setInterval(unsigned long mSec),
    setNextRunTime(unsigned long mSec),
    disable(),
    enable();
  Callback
    run();

  bool
    enabled();

  unsigned long
    nextRunTime();

private:
  bool
    _enabled;

  Callback
    _callback;

  unsigned long
    _runNextAt,
    _timeBetweenRuns;

};



class StuScheduler {

public:
  StuScheduler();

  void
    addTask(Task *t),
    run();



private:
  Task
    *_Task[20];

  uint8_t
    _tItr;



};

#endif
