#include "stu_scheduler.h"


StuScheduler s;
Task testTask(&testFunc);


void setup(){
  Serial.begin(115200);

  Serial.println(F("Serial begin"));
  testTask.run();
}


void loop(){



}


void testFunc(){
  Serial.println(F("It works!"));
}
