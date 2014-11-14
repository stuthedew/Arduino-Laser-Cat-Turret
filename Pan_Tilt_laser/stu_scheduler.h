
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

  Task(void (*callback)(), unsigned long interval=100, bool enable=1);

  void
    resetPeriodic(),
    setInterval(unsigned long mSec),
    setNextRunTime(unsigned long mSec),
    disable(),
    enable(),
    run(),
    changeCallback(void (*callback)());

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
    run(),
    restart();



private:
  Task
    *_Task[20];

  uint8_t
    _tItr;
};


}
