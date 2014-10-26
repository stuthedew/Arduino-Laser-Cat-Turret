
/**************************************************************************/
/*!
    @file     stu_scheduler.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple scheduler to manage various events (e.g. sleep). Addapted from
    "Making Embedded Systems" by Elecia White.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/\


#include "stu_scheduler.h"

void resetPeriodic(struct Task *t){

  t->runNextAt = t->timeBetweenRuns + millis();
}

void TaskSetNextTime(struct Task *t, unsigned long timeFromNow){
  t->timeBetweenRuns = timeFromNow;
  t->runNextAt = t->timeBetweenRuns + millis();

}


void TaskDisable(struct Task *t){
  t->enabled = 0;

}


void TaskEnable(struct Task *t){
  t->enabled = 1;
  resetPeriodic(t);
}


StuScheduler::StuScheduler(uint8_t arySize = 20):_tArraySize(arySize){

}

void StuScheduler::addTask(struct Task *t){


}

void StuScheduler::run(){

  for(int i = 0; i < _tArraySize; i++){
    if(_Task[i]->enabled && _Task[i]->runNextAt >= millis()){
      _Task[i]->callback;
      resetPeriodic(_Task[i]);
    }
  }

}
