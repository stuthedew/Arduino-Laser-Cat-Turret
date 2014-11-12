#ifndef _STUTILITY_H_
#define _STUTILITY_H_

#include "stu_scheduler.h"
#include "stu_gauss.h"


extern Task pauseTask;
extern Task restTask;
extern Task sleepTask;
extern StuGauss gauss;


//halt laser at certain spot for a few moments at this time
void setNextPauseTime(unsigned long avg_sec_to_pause=15, double variance=12){

  unsigned long temp = gauss.gRandom(avg_sec_to_pause, variance)*1000;
/*
  Serial.print(F("Next pause in "));
  Serial.print(temp/1000);
  Serial.println(F(" seconds.\n"));
*/
  pauseTask.setInterval(temp);

}

//turn off laser for a few moments at this time
void setNextRestTime(unsigned long avg_sec_to_rest=360, double variance=60){
  unsigned long temp = gauss.gRandom(avg_sec_to_rest, variance)*1000;
/*
  Serial.print(F("Next rest in "));
  Serial.print(temp/1000);
  Serial.println(F(" seconds.\n"));
*/
  restTask.setInterval(temp);
}


//turn of laser for a minutes to hours at this time
void setNextSleepTime(unsigned long avg_min_to_sleep=10, double variance = 3){
  unsigned long mSecToSleep = max(1, gauss.gRandom(avg_min_to_sleep, variance))*60000;

//  unsigned long mSecToSleep = gauss.gRandom(avg_min_to_sleep, variance)*60000;
/*
  Serial.print(F("Next sleep in "));
  Serial.print(mSecToSleep/1000);
  Serial.println(F(" seconds.\n"));
*/
  sleepTask.setInterval(mSecToSleep);

}




#endif
