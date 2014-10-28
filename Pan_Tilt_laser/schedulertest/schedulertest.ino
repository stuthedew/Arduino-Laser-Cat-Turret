#include "stu_scheduler.h"


StuScheduler s;
Task testTask1(&testFunc1, 300);
Task testTask2(&testFunc2, 700);
Task testTask3(&testFunc3, 2000);

void setup(){
  Serial.begin(115200);
  s.addTask(&testTask1);
  s.addTask(&testTask2);
  s.addTask(&testTask3);

  
  Serial.println(F("Serial begin"));
}


void loop(){
s.run();
delay(10);

}


void testFunc1(){
  Serial.println(F("1 works!"));
}


void testFunc2(){
  Serial.println(F("2 works!"));
}


void testFunc3(){
  Serial.println(F("3 works!"));
}
