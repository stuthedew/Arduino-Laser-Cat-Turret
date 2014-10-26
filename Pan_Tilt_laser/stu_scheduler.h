
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


struct Task;
typedef void (*TaskCallback)(struct Task *);
typedef struct Task{
  unsigned long
    runNextAt,
    timeBetweenRuns;

  TaskCallback
    callback;

  bool
    enabled;
};

void
  TaskresetPeriodic(struct Task *t),
  TaskSetNextTime(struct Task *t, unsigned long timeFromNow),
  TaskDisable(struct Task *t),
  TaskEnable(struct Task *t);


class StuScheduler {

public:
  StuScheduler(uint8_t);

  void
    addTask(struct Task *t),
    run();



private:
  Task
    *_Task[20];

  const uint8_t
    _tArraySize;



};

#endif
