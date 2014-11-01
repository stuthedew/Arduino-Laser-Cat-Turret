/**************************************************************************/
/*!
    @file     Pan_Tilt_laser.ino
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
*/
/**************************************************************************/



#include <Servo.h>
#include <math.h>
#include "stuServo.h"
#include "stuPanTilt.h"
#include "stuLaser.h"
#include "stu_scheduler.h"
#include "missileswitch.h"
#include "stu_gauss.h"
#include <AVector.h>
#include <Gaussian.h>


#define BAUD_RATE 115200

#define MS_SWITCH_PIN 3
#define MS_LED_PIN 4

#define LASER_PIN 5

#define SERVO_X_PIN A0
#define SERVO_Y_PIN A1


#define DIRECTION_CHANGE_PROBABILITY 15


//X Position: lower numbers == Right
//Y Position: lower numbers == Up

panTiltPos_t panTiltX(50, 120);
panTiltPos_t panTiltY(7, 45);


PanTilt panTilt(SERVO_X_PIN, &panTiltX, SERVO_Y_PIN, &panTiltY);

StuLaser laser(LASER_PIN);


Missileswitch mSwitch(MS_SWITCH_PIN, MS_LED_PIN);



unsigned long timeFunc1(StuLaser& L, int x, int y){
  unsigned long startTime, endTime;

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
  return abs(a2 - a1) * 100 / a2;

}


void diffCheck(double diff, double fastDeg, double oldDeg){
  if(diff > 1){
    Serial.println(F("Greater than 1% difference!"));
    Serial.print(F("Old acos:    "));
    Serial.println(oldDeg);
    Serial.print(F("Fast acos:   "));
    Serial.println(fastDeg);
    Serial.print(F("DIFFERENCE:  "));
    Serial.println(diff);
    Serial.println();
  }

}


void setup() {
  Serial.begin(BAUD_RATE);
  panTilt.begin();
  laser.begin();

  mSwitch.begin();
  randomSeed(analogRead(4));
  laser.setOrigin(0, 99);

  laser.setDotPosition(0, 30);

  //Serial.print(F("Calc angles time: "));

//Serial.println(timeFunc3());

//Serial.println(laser.hAngleDeg());
//Serial.println(laser.vAngleDeg());

//Serial.print(F("lerp time: "));

//Serial.println(timeFunc3());


//Serial.println(p0.lerp(&p1, 3));



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

for(int i = -1000; i < 1000; i+= 20){
  unsigned long tempOld = 0;
  unsigned long tempFast = 0;
  int x = random(0, 180);
  int y = random(0, 180);
  oldAcos.current.x = x;
  oldAcos.current.y = y;
  fastAcos.current.x = x;
  fastAcos.current.y = y;

  fastAcos.current.time = timeFunc2(laser, x, y);

  fastAcos.current.hDeg = laser.hAngleDeg();
  fastAcos.current.vDeg = laser.vAngleDeg();

  oldAcos.current.time = timeFunc1(laser, x, y);
  oldAcos.current.hDeg = laser.hAngleDeg();
  oldAcos.current.vDeg = laser.vAngleDeg();

  setVals(&fastAcos, &oldAcos);
  setVals(&oldAcos, &fastAcos);

  double hDiff = getAngleDiff(fastAcos.current.hDeg, oldAcos.current.hDeg);
  double vDiff = getAngleDiff(fastAcos.current.vDeg, oldAcos.current.vDeg);

  diffCheck(hDiff, fastAcos.current.hDeg, oldAcos.current.hDeg);

itr++;
}

oldAcos.avgCase.time /= oldAcos.itr;

fastAcos.avgCase.time /= fastAcos.itr;



Serial.println(F("Fast acos average: "));
Serial.println(fastAcos.avgCase.time);
Serial.println(F("\nold acos average: "));
Serial.println(oldAcos.avgCase.time);
Serial.println();

Serial.println(F("Fast acos Best Case: "));
Serial.print(F("Value: "));
Serial.print(fastAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.bestCase.y);
Serial.print(F("Time: "));
Serial.println(fastAcos.bestCase.time);
Serial.print(F("old acos time for "));
Serial.print(fastAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.bestCase.y);
Serial.println(F(": "));
Serial.print(fastAcos.bestComp.time);
Serial.println();

Serial.println(F("Fast acos Worst Case: "));
Serial.print(F("Value: "));
Serial.print(fastAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.worstCase.y);
Serial.print(F("Time: "));
Serial.println(fastAcos.worstCase.time);
Serial.print(F("old acos time for "));
Serial.print(fastAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(fastAcos.worstCase.y);
Serial.print(F(": "));
Serial.println(fastAcos.worstComp.time);
Serial.println();

Serial.println(F("Old acos Best Case: "));
Serial.print(F("Value: "));
Serial.print(oldAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.bestCase.y);
Serial.print(F("Time: "));
Serial.println(oldAcos.bestCase.time);
Serial.print(F("old acos time for "));
Serial.print(oldAcos.bestCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.bestCase.y);
Serial.print(F(": "));
Serial.println(oldAcos.bestComp.time);
Serial.println();

Serial.println(F("Old acos Worst Case: "));
Serial.print(F("Value: "));
Serial.print(oldAcos.worstCase.x);
Serial.print(F(", "));
Serial.print(oldAcos.worstCase.y);
Serial.print(F("Time: "));
Serial.println(fastAcos.worstCase.time);
Serial.print(F("old acos time for "));
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
}
