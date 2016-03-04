/**************************************************************************!
    @file     stu_scheduler.cpp                                           *
    @author   Stuart Feichtinger                                          *
    @license  MIT (see license.txt)                                       *
 *
    Simple scheduler to manage various events (e.g. sleep). Addapted from *
    "Making Embedded Systems" by Elecia White.                            *
 *
 *
    @section  HISTORY                                                     *
    v1.0 - First release                                                  *
 *
 **************************************************************************/


#include "stu_scheduler.h"

Task::Task(void(*cbFunc)(), unsigned long interval, bool enable) : _callback(
    cbFunc), _timeBetweenRuns(interval), _enabled(enable) {}

void Task::resetPeriodic() {
  _runNextAt = _timeBetweenRuns + millis();
}

void Task::setInterval(unsigned long mSec) {
  _timeBetweenRuns = mSec;
  _runNextAt       = _timeBetweenRuns + millis();
}

unsigned long Task::nextRunTime() {
  return _runNextAt;
}

void Task::setNextRunTime(unsigned long mSec) {
  _runNextAt = mSec;
}

void Task::disable() {
  _enabled = 0;
}

void Task::enable() {
  _enabled = 1;
  resetPeriodic();
}

void Task::run() {
  _callback();
}

bool Task::enabled() {
  return _enabled;
}

void Task::changeCallback(void (*cbFunc)()) {
  _callback = cbFunc;
}

StuScheduler::StuScheduler() {
  _tItr = 0;
}

void StuScheduler::addTask(Task *t) {
  _Task[_tItr] = t;
  _tItr++;
}

void StuScheduler::restart() {
  for (uint8_t i = 0; i < _tItr; i++) {
    if (_Task[i]->enabled()) {
      _Task[i]->resetPeriodic();
    }
  }
}

void StuScheduler::run() {
  for (uint8_t i = 0; i < _tItr; i++) {
    if (_Task[i]->enabled() && (_Task[i]->nextRunTime() <= millis())) {
      _Task[i]->run();
      _Task[i]->resetPeriodic();
    }
  }
}
