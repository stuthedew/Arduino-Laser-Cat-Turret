/****************************************************************************!
    @file     Pan_Tilt_laser.ino                                            *
    @author   Stuart Feichtinger                                            *
    @license  MIT (see license.txt)                                         *
                                                                            *
    Main program running an automatic laser-turret cat toy using a pan-tilt *
    rig (https://www.adafruit.com/products/1967) and laser diode            *
    (https://www.adafruit.com/products/1054). Random laser path, speed      *
    and pauses determined by homebrew Markov chain algorithm.               *
                                                                            *
                                                                            *
    @section  HISTORY                                                       *
    v1.0.0 - First release                                                  *
    v1.1.0 - Added Markov-based speed control                               *
    v1.2.0 - Added Markov-based pause length/frequency                      *
    v1.2.1 - Added Heartbeat during pause                                   *
    v1.3.0 - Added laser shake to enhance cat enjoyment                     *
    v1.3.1 - Changed probability polling frequency to once a second         *
    v1.4.0 - Added ON/OFF Missile Switch from Sparkfun                      *
    v1.5.0 - Added scheduler and gaussian library for random values         *
    v1.5.1 - Converted speed and direction function to scheduled task       *
 ****************************************************************************/



#define PROGMEM_ __attribute__((section(".progmem.data")))

#include <Servo.h>
#include <math.h>
#include "stuServo.h"
#include "stuPanTilt.h"
#include "stuLaser.h"
#include "stu_scheduler.h"
#include "missileswitch.h"
#include "stu_gauss.h"
#include <avr/pgmspace.h>
#include <AVector.h>
#include <Gaussian.h>


#define BAUD_RATE 115200

#define MS_SWITCH_PIN 3
#define MS_LED_PIN 4

#define LASER_PIN 5

#define SERVO_X_PIN A0
#define SERVO_Y_PIN A1


#define DIRECTION_CHANGE_PROBABILITY 15


// FLASH_STRING(vTitle, "Vertical");
// FLASH_STRING(hTitle, "Horizontal");

const char vTitle[] PROGMEM = "Vertical";
const char hTitle[] PROGMEM = "Horizontal";

// X Position: lower numbers == Right
// Y Position: lower numbers == Up

panTiltPos_t panTiltX(50, 120);
panTiltPos_t panTiltY(7, 45);


PanTilt panTilt(SERVO_X_PIN, &panTiltX, SERVO_Y_PIN, &panTiltY);

StuLaser laser(LASER_PIN);


Missileswitch mSwitch(MS_SWITCH_PIN, MS_LED_PIN);


unsigned long timeFunc1(StuLaser& L, int x, int y) {
  unsigned long startTime, endTime;

  startTime = micros();
  L.setDotPosition(x, y);
  endTime = micros();

  return endTime - startTime;
}

unsigned long timeFunc2(StuLaser& L, int x, int y) {
  unsigned long startTime, endTime;

  startTime = micros();
  L.setDotPositionFast(x, y);
  endTime = micros();

  return endTime - startTime;
}

AVector p0(2, 8);
AVector p1(5, 17);

unsigned long timeFunc3() {
  unsigned long startTime, endTime;

  // Serial.print(F("lerp time: "));
  startTime = micros();
  p0.lerp(&p1, 1.52);
  endTime = micros();

  return endTime - startTime;
}

struct valTime_s {
  int
    x,
    y;

  unsigned long
    time;

  double
    vDeg,
    hDeg;
};

struct case_s {
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

  case_s() : hName(hTitle), vName(vTitle) {}
} oldAcos, fastAcos;


void setVals(struct case_s *set, struct case_s *comp) {
  if (set->worstCase.time < set->current.time) {
    set->worstCase.time = set->current.time;
    set->worstCase.x    = set->current.x;
    set->worstCase.y    = set->current.y;

    set->worstComp.time = comp->current.time;
    set->worstCase.x    = set->current.x;
    set->worstCase.y    = set->current.y;
  }

  if (set->bestCase.time > set->current.time) {
    set->bestCase.time = set->current.time;
    set->bestCase.x    = set->current.x;
    set->bestCase.y    = set->current.y;

    set->bestComp.time = comp->current.time;
    set->bestComp.x    = set->current.x;
    set->bestComp.y    = set->current.y;
  }
  set->avgCase.time += set->current.time;
  set->itr          += 1;
}

inline double getAngleDiff(double a1, double a2) {
  return abs(a2 - a1); // * 100 / a2;
}

int offItr = 0;

void angleCheck(double      diff,
                double      fastDeg,
                double      oldDeg,
                const char *s,
                int         x,
                int         y) {
  if (diff > 2) {
    Serial.print(F("Greater than 1% difference in "));

    while (pgm_read_byte(s) != '\0') {
      Serial.print((char)pgm_read_byte(s++));
    }
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

void diffCheck(struct case_s *fast, struct case_s *old) {
  double hDiff = getAngleDiff(fast->current.hDeg, old->current.hDeg);
  double vDiff = getAngleDiff(fast->current.vDeg, old->current.vDeg);

  angleCheck(hDiff,
             fast->current.hDeg,
             old->current.hDeg,
             fast->hName,
             fast->current.x,
             fast->current.y);
  angleCheck(vDiff,
             fast->current.vDeg,
             old->current.vDeg,
             fast->vName,
             fast->current.x,
             fast->current.y);
}

void setup() {
  Serial.begin(BAUD_RATE);
  panTilt.begin();
  laser.begin();

  mSwitch.begin();
  randomSeed(analogRead(4));
  laser.setOrigin(0, 99);

  laser.setDotPositionFast(-100, 123);

  // Serial.print(F("Calc angles time: "));

  // Serial.println(timeFunc3());

  // Serial.println(laser.hAngleDeg());
  // Serial.println(laser.vAngleDeg());

  // Serial.print(F("lerp time: "));

  // Serial.println(timeFunc3());


  // Serial.println(p0.lerp(&p1, 3));

  Serial.print(F("Fast acos value: ("));
  Serial.print(F("-100, 123"));
  Serial.print(F(")\nDegree: "));
  Serial.println(laser.vAngleDeg());
  laser.setDotPosition(-100, 123);
  Serial.print(F("old acos value :"));
  Serial.println(laser.vAngleDeg());

  Serial.println();

  while (1) ;


  laser.fire(1);
  int itr = 0;

  oldAcos.itr             = 0;
  oldAcos.avgCase.time    = 0;
  fastAcos.itr            = 0;
  fastAcos.avgCase.time   = 0;
  fastAcos.worstCase.time = 0;
  fastAcos.bestCase.time  = 1000;
  oldAcos.worstCase.time  = 0;
  oldAcos.bestCase.time   = 1000;
  Serial.println();
  Serial.println();

  for (int i = 0; i < 1000; i += 20) {
    unsigned long tempOld  = 0;
    unsigned long tempFast = 0;
    int x                  = random(-100, -100);
    int y                  = random(0, 500);
    oldAcos.current.x  = x;
    oldAcos.current.y  = y;
    fastAcos.current.x = x;
    fastAcos.current.y = y;

    fastAcos.current.time = timeFunc2(laser, x, y);

    fastAcos.current.hDeg = laser.hAngleDeg();
    fastAcos.current.vDeg = laser.vAngleDeg();

    oldAcos.current.time = timeFunc1(laser, x, y);
    oldAcos.current.hDeg = round(laser.hAngleDeg());
    oldAcos.current.vDeg = round(laser.vAngleDeg());

    setVals(&fastAcos, &oldAcos);
    setVals(&oldAcos,  &fastAcos);


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


  while (1);

  mSwitch.ledState(1);
  panTiltX.angle = laser.hAngleDeg();
  panTiltY.angle = laser.vAngleDeg();
  panTilt.updateAngles();

  delay(2000);

  mSwitch.ledState(1);
  laser.fire(0);
}

void loop() {}

int  gethAngle() {
  return 1;
}
