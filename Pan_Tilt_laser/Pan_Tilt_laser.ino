/**************************************************************************/
/*!
@file     Pan_Tilt__laser.ino
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Main program running an automatic laser-turret cat toy using a pan-tilt
rig (https://www.adafruit.com/products/1967) and laser diode
(https://www.adafruit.com/products/1054). Random laser path, speed
and pauses determined by homebrew Markov chain algorithm.


@section  HISTORY
v1.0.0 - First release
v1.1.0 - Added Markov-based speed control
v1.2.0 - Added Markov-based pause length/frequency
v1.2.1 - Added Heartbeat during pause
v1.3.0 - Added laser shake to enhance cat enjoyment
v1.3.1 - Changed probability polling frequency to once a second
v1.4.0 - Added ON/OFF Missile Switch from Sparkfun
v1.5.0 - Added scheduler and gaussian library for random values
v1.5.1 - Converted speed and direction function to scheduled task
v1.6.0 - Switched Markov speed change to Markov handler using circuler
linked list.
v1.6.1 - Switched Markov state to Markov handler
v1.7.0 - Moved control of timing to master Pan-Tilt Class
v1.8.0 - Switched to a state-machine model.
v1.9.0 - Final Version
v1.9.1 - Tweaked minimum Y servo value
v1.9.2 - Fixed bug where pauseTask was still enabled when off.
*/
/**************************************************************************/

<<<<<<< HEAD
#define PROGMEM_ __attribute__((section(".progmem.data")))

#include <Servo.h>
#include <math.h>
#include "stuServo.h"
=======
#include "panTilt_config.h"
>>>>>>> master
#include "stuPanTilt.h"
#include <Time.h>
#include <Servo.h>
#include "stuMarkov.h"
#include "stuLaser.h"
#include "stu_scheduler.h"
#include "stu_gauss.h"
#include <avr/pgmspace.h>
#include <AVector.h>
#include <Gaussian.h>
#include "stu_dial.h"


<<<<<<< HEAD

#define BAUD_RATE 115200
=======
#define MIN_LOOP_TIME 0
>>>>>>> master


int markovShakeState = 1;
int changeVal;

LinkedMarkov lmSpeed;
LinkedMarkov lmShake;
LinkedMarkov lmPause;

PanTilt panTilt( SERVO_X_PIN, SERVO_Y_PIN );


<<<<<<< HEAD

//FLASH_STRING(vTitle, "Vertical");
//FLASH_STRING(hTitle, "Horizontal");

const char vTitle[]PROGMEM = "Vertical";
const char hTitle[]PROGMEM = "Horizontal";
=======
Task pauseTask(&pauseCB);
Task updateMarkovTask(&updateMarkov, 750, 1);
>>>>>>> master

void updateMarkov(){
  changeVal = lmSpeed.getNextValue();
  markovShakeState = lmShake.getNextValue();
  updateMarkovTask.enable();

<<<<<<< HEAD
panTiltPos_t panTiltX(50, 120);
panTiltPos_t panTiltY(7, 45);
=======
}
>>>>>>> master

//halt laser at certain spot for a few moments at this time
void setNextPauseTime(unsigned long avg_sec_to_pause=15, double variance=6){

  unsigned long temp = gauss.gRandom(avg_sec_to_pause, variance)*1000;

  #ifdef SERIAL_DEBUG
  #ifdef TIME_DEBUG
  MY_SERIAL.print(F("Next pause in "));
  MY_SERIAL.print(temp/1000);
  MY_SERIAL.println(F(" seconds.\n"));
  #endif
  #endif

<<<<<<< HEAD

Missileswitch mSwitch(MS_SWITCH_PIN, MS_LED_PIN);


=======
  pauseTask.setInterval(temp);
  pauseTask.enable();

}

void offCB(){

  pauseTask.disable();
>>>>>>> master

unsigned long timeFunc1(StuLaser& L, int x, int y){
  unsigned long startTime, endTime;

<<<<<<< HEAD
  startTime = micros();
  L.setDotPosition(x, y);
  endTime = micros();

  return endTime - startTime;
}

unsigned long timeFunc2(StuLaser& L, int x, int y){
  unsigned long startTime, endTime;

  startTime = micros();
  L.setDotPositionFast(x, y);
  endTime = micros();

  return endTime - startTime;
  }
AVector p0(2, 8);
AVector p1(5, 17);

unsigned long timeFunc3(){
  unsigned long startTime, endTime;
//Serial.print(F("lerp time: "));
  startTime = micros();
  p0.lerp(&p1, 1.52);
  endTime = micros();

  return endTime - startTime;
}

struct valTime_s{
  int
    x,
    y;

  unsigned long
    time;

  double
    vDeg,
    hDeg;

};

struct case_s{
  struct valTime_s
    bestCase,
    bestComp,
    avgCase,
    worstCase,
    worstComp,
    current,
    currentComp;

  int
    itr;

  const char
      *hName,
      *vName;

  case_s():hName(hTitle), vName(vTitle){}

}oldAcos, fastAcos;



void setVals(struct case_s *set, struct case_s *comp){

  if(set->worstCase.time < set->current.time){

      set->worstCase.time = set->current.time;
      set->worstCase.x = set->current.x;
      set->worstCase.y = set->current.y;

      set->worstComp.time = comp->current.time;
      set->worstCase.x = set->current.x;
      set->worstCase.y = set->current.y;
  }
  if(set->bestCase.time > set->current.time){

      set->bestCase.time = set->current.time;
      set->bestCase.x = set->current.x;
      set->bestCase.y = set->current.y;

      set->bestComp.time = comp->current.time;
      set->bestComp.x = set->current.x;
      set->bestComp.y = set->current.y;
  }
  set->avgCase.time += set->current.time;
  set->itr += 1;
}


inline double getAngleDiff(double a1, double a2){
  return abs(a2 - a1);// * 100 / a2;

}

int offItr = 0;

void angleCheck(double diff, double fastDeg, double oldDeg, const char *s, int x, int y){
=======
void runCB(){

  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("RUN CALLBACK"));
  #endif

  setNextPauseTime();

}

void restCB(){

  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("REST CALLBACK"));
  #endif

  return;
}


void pauseCB(){

  pauseTask.disable();

  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("PAUSE CALLBACK"));
  #endif

  panTilt.pause( markovPause(), !!(random()%4) );

  setNextPauseTime();

}

void panTiltCB(){

  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("Main Sketch PanTilt Callback!!!! "));
  #endif


  panTilt.callback();

}

void setup() {


  #ifdef SERIAL_DEBUG
  MY_SERIAL.begin(BAUD_RATE);
  MY_SERIAL.println(F("setup starting..."));
  #endif

  panTilt.begin();
  panTilt.setStateCallback(STATE_OFF, &offCB);
  panTilt.setStateCallback(STATE_RUN, &runCB);
  panTilt.setStateCallback(STATE_REST, &restCB);

  Task* taskPtr = panTilt.getTaskPtr();

  taskPtr->changeCallback(panTiltCB);


  //        addLinkToBack(speed, previous_state_probability, next_state_probability)
  lmSpeed.addLinkToBack( 1,  5, 35 ); // Slow
  //                           ^  ||
  //                           |  \/
  lmSpeed.addLinkToBack( 2, 25, 35 ); // Med
  //                           ^  ||
  //                           |  \/
  lmSpeed.addLinkToBack( 3, 35, 25 ); // Fast
  //                           ^  ||
  //                           |  \/
  //                          | Slow |


  //        addLinkToBack(state, previous_state_probability, next_state_probability)
  lmShake.addLinkToBack( 1,  0, 2 ); // No shake
  //                           ^  ||
  //                           |  \/
  lmShake.addLinkToBack( 2, 40, 0 ); // Shake

  randomSeed(analogRead(5));


  scheduler.addEvent(&pauseTask);
  scheduler.addEvent(&updateMarkovTask);



  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("setup complete"));
  #endif

  setNextPauseTime();

}


void loop(){


  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(panTilt.getState());
  #endif


  if( panTilt.getState() == STATE_RUN ){
    if(!pauseTask.enabled()){
      pauseTask.enable();
>>>>>>> master

  if(diff > 2){
    Serial.print(F("Greater than 1% difference in "));
    while (pgm_read_byte(s) != '\0'){
      Serial.print((char)pgm_read_byte(s++));
    }
<<<<<<< HEAD
    Serial.print(F(" angle for ("));
    Serial.print(x);
    Serial.print(F(", "));
    Serial.print(y);
    Serial.print(F(")!!!\nOld acos:    "));
    Serial.print(oldDeg);
    Serial.print(F("\nFast acos:   "));
    Serial.print(fastDeg);
    Serial.print(F("\nDIFFERENCE:  "));
    Serial.println(diff);
    Serial.println();
    Serial.println();
    offItr++;
  }
}

void diffCheck(struct case_s *fast, struct case_s *old){
  double hDiff = getAngleDiff(fast->current.hDeg, old->current.hDeg);
  double vDiff = getAngleDiff(fast->current.vDeg, old->current.vDeg);
=======

    panTilt.posX.angle = getDeltaPosition(&panTilt.posX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posX.angle;
    panTilt.posY.angle = getDeltaPosition(&panTilt.posY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posY.angle;
>>>>>>> master

  angleCheck(hDiff, fast->current.hDeg, old->current.hDeg, fast->hName, fast->current.x, fast->current.y);
  angleCheck(vDiff, fast->current.vDeg, old->current.vDeg, fast->vName, fast->current.x, fast->current.y);

<<<<<<< HEAD
}
=======
    if(markovShakeState == 2){
      panTilt.shake();
    }
  }
>>>>>>> master

  else if(pauseTask.enabled()){

<<<<<<< HEAD
void setup() {

  Serial.begin(BAUD_RATE);
  panTilt.begin();
  laser.begin();

  mSwitch.begin();
  randomSeed(analogRead(4));
  laser.setOrigin(0, 99);
=======

  }
  scheduler.run();
  panTilt.update();
  delay(5);

}
>>>>>>> master

  laser.setDotPositionFast(-100, 123);

  //Serial.print(F("Calc angles time: "));

<<<<<<< HEAD
//Serial.println(timeFunc3());

//Serial.println(laser.hAngleDeg());
//Serial.println(laser.vAngleDeg());

//Serial.print(F("lerp time: "));

//Serial.println(timeFunc3());


//Serial.println(p0.lerp(&p1, 3));
=======
  int prob = changeProb;

  if(pt->dir == 0){
    pt->dir = 1;
  }

  if((pt->dir == 1 && pt->angle >= pt->midAngle) || (pt->dir == -1 && pt->angle <= pt->midAngle)){
    prob += abs(pt->midAngle - pt->angle) * max(pt->probOffset, 1);

  }

  if(random(1001) <= prob << 1 || (pt->angle >= pt->maxAngle && pt->dir == 1) || pt->angle <= pt->minAngle && pt->dir == -1){
    pt->dir *= -1;
  }
  return pt->dir;
}
>>>>>>> master

Serial.print(F("Fast acos value: ("));
Serial.print(F("-100, 123"));
Serial.print(F(")\nDegree: "));
Serial.println(laser.vAngleDeg());
laser.setDotPosition(-100, 123);
Serial.print(F("old acos value :"));
Serial.println(laser.vAngleDeg());

<<<<<<< HEAD
Serial.println();
=======
int getDeltaPosition(panTiltPos_t *pt, int funcChangeVal, int changeProb){

  int tempVal = getMarkovDirection(pt, changeProb);
>>>>>>> master

while(1);



<<<<<<< HEAD
  laser.fire(1);
  int itr = 0;

  oldAcos.itr = 0;
  oldAcos.avgCase.time = 0;
  fastAcos.itr = 0;
  fastAcos.avgCase.time = 0;
  fastAcos.worstCase.time = 0;
  fastAcos.bestCase.time = 1000;
  oldAcos.worstCase.time = 0;
  oldAcos.bestCase.time = 1000;
  Serial.println();
  Serial.println();

for(int i = 0; i < 1000; i+= 20){
  unsigned long tempOld = 0;
  unsigned long tempFast = 0;
  int x = random(-100, -100);
  int y = random(0, 500);
  oldAcos.current.x = x;
  oldAcos.current.y = y;
  fastAcos.current.x = x;
  fastAcos.current.y = y;

  fastAcos.current.time = timeFunc2(laser, x, y);

  fastAcos.current.hDeg = laser.hAngleDeg();
  fastAcos.current.vDeg = laser.vAngleDeg();
=======
>>>>>>> master

  oldAcos.current.time = timeFunc1(laser, x, y);
  oldAcos.current.hDeg = round(laser.hAngleDeg());
  oldAcos.current.vDeg = round(laser.vAngleDeg());

<<<<<<< HEAD
  setVals(&fastAcos, &oldAcos);
  setVals(&oldAcos, &fastAcos);


  diffCheck(&fastAcos, &oldAcos);

itr++;
}

oldAcos.avgCase.time /= oldAcos.itr;

fastAcos.avgCase.time /= fastAcos.itr;

double offPercent = offItr * 100;
offPercent /= itr;

Serial.print(F("% off: "));
Serial.println(offPercent);


Serial.println(F("Fast acos average: "));
Serial.println(fastAcos.avgCase.time);
Serial.print(F("\nold acos average: "));
Serial.println(oldAcos.avgCase.time);
Serial.println();

Serial.println(F("Fast acos Best Case: "));
Serial.print(F("Value: ("));
Serial.print(fastAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.bestCase.y);
Serial.print(F(")\nTime: "));
Serial.println(fastAcos.bestCase.time);
Serial.print(F("old acos time for ("));
Serial.println(fastAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.bestCase.y);
Serial.print(F("): "));
Serial.print(fastAcos.bestComp.time);
Serial.println();

Serial.println(F("Fast acos Worst Case: "));
Serial.print(F("Value: ("));
Serial.print(fastAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.worstCase.y);
Serial.print(F(")\nTime: "));
Serial.println(fastAcos.worstCase.time);
Serial.print(F("old acos time for ("));
Serial.print(fastAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.worstCase.y);
Serial.print(F("): "));
Serial.println(fastAcos.worstComp.time);
Serial.println();

Serial.println(F("Old acos Best Case: "));
Serial.print(F("Value: ("));
Serial.print(oldAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.bestCase.y);
Serial.print(F(")\nTime: "));
Serial.println(oldAcos.bestCase.time);
Serial.print(F("fast acos time for ("));
Serial.print(oldAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.bestCase.y);
Serial.print(F("): "));
Serial.println(oldAcos.bestComp.time);
Serial.println();

Serial.println(F("Old acos Worst Case: "));
Serial.print(F("Value: ("));
Serial.print(oldAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.worstCase.y);
Serial.print(F(")\nTime: "));
Serial.println(fastAcos.worstCase.time);
Serial.print(F("fast acos time for ("));
Serial.print(oldAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.worstCase.y);
Serial.println();

double tempPer = oldAcos.avgCase.time - fastAcos.avgCase.time;
tempPer *= 100;
tempPer /= oldAcos.avgCase.time;
Serial.print(F("Improvment of "));
Serial.print(tempPer);
Serial.println(F("%!"));


  while(1);

  mSwitch.ledState(1);
  panTiltX.angle = laser.hAngleDeg();
  panTiltY.angle = laser.vAngleDeg();
  panTilt.updateAngles();

  delay(2000);

  mSwitch.ledState(1);
  laser.fire(0);
}

void loop() {

}


int gethAngle(){

  return 1;
=======
  if(val < 25){
    return random(1500, 2000);
  }
  else if(val < 90){
    return random(1000, 1500);
  }
  else{
    return random(500, 750);
  }

>>>>>>> master
}
