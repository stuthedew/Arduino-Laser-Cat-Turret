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
    v1.6.0 - Switched Markov speed change to Markov handler using circuler
             linked list.
    v1.6.1 - Switched Markov state to Markov handler
*/
/**************************************************************************/

#include "panTilt_config.h"
#include <Servo.h>
#include <Gaussian.h>

using namespace stu;

int markovShakeState = 1;
int changeVal;

LinkedMarkov lmSpeed;
LinkedMarkov lmShake;

//X Position: lower numbers == Right
//Y Position: lower numbers == Up
panTiltPos_t panTiltX(0, 50, 120, -30);
panTiltPos_t panTiltY(0, 7, 45, 0, 10);


PanTilt panTilt(SERVO_X_PIN, &panTiltX, SERVO_Y_PIN, &panTiltY);

StuLaser laser(LASER_PIN);

Missileswitch mSwitch(MS_SWITCH_PIN, MS_LED_PIN);


Task pauseTask(&pauseCB);
Task restTask(&restCB);
Task sleepTask(&sleepCB);
Task speedAndDirTask(&updateSpeedAndDir, 750);

StuScheduler schedule;

void updateSpeedAndDir(){
  changeVal = lmSpeed.getNextValue();
  markovShakeState = lmShake.getNextValue();

}


void pauseCB(){
  //Serial.println(F("Pause Callback!"));
  delay(markovPause());
  setNextPauseTime();
}

void restCB(){
  //Serial.println(F("Rest Callback!"));
  mSwitch.ledState(0);
  sleep(5, 10);
  mSwitch.ledState(1);
  setNextPauseTime();
  setNextRestTime();
}

void sleepCB(){
  //Serial.println(F("Sleep Callback!"));
  mSwitch.ledState(0);
  sleep(1800, 2400); //sleep between 30 and 40 minutes
  mSwitch.ledState(1);
  setNextPauseTime();
  setNextRestTime();
  setNextSleepTime();
}



void setup() {
  Serial.begin(BAUD_RATE);

  mSwitch.begin();

  schedule.addTask(&pauseTask);
  schedule.addTask(&restTask);
  schedule.addTask(&sleepTask);
  schedule.addTask(&speedAndDirTask);

//        addLinkToBack(speed, previous_state_probability, next_state_probability)
  lmSpeed.addLinkToBack( 2,  5, 35 ); // Slow
//                           ^  ||
//                           |  \/
  lmSpeed.addLinkToBack( 4, 25, 35 ); // Med
//                           ^  ||
//                           |  \/
  lmSpeed.addLinkToBack( 6, 35, 25 ); // Fast
//                           ^  ||
//                           |  \/
//                          | Slow |


//        addLinkToBack(state, previous_state_probability, next_state_probability)
  lmShake.addLinkToBack( 1,  0, 5 ); // No shake
//                           ^  ||
//                           |  \/
  lmShake.addLinkToBack( 2, 30, 0 ); // Shake

  randomSeed(analogRead(5));

  mSwitch.heartBeat(3);

  if(!mSwitch.switchState()) {
    mSwitch.ledState(0);
    while(!mSwitch.switchState()){
      delay(50);
      }
  }

  panTilt.begin();

  Serial.println(F("setup starting..."));
  mSwitch.ledState(1);


  laser.begin();

  panTiltX.angle = panTiltX.minAngle;
  panTiltY.angle = panTiltY.minAngle;
  panTilt.updateAngles();
  delay(500);

  panTiltX.angle = panTiltX.maxAngle;
  panTiltY.angle = panTiltY.maxAngle;
  panTilt.updateAngles();
  delay(500);


  panTiltX.angle = panTiltX.midAngle;
  panTiltY.angle = panTiltY.midAngle;
  panTilt.updateAngles();

  delay(1000);
  laser.fire(1);

  Serial.println(F("setup complete"));

  setNextPauseTime();
  setNextRestTime();
  setNextSleepTime();
}

void loop() {

  schedule.run();

  //check if switch is on or off and pause if off
  if(!mSwitch.switchState()){

    #ifdef MAIN_DEBUG
      Serial.print(F("Switch is off!"));
    #endif

    laser.fire(0);
    mSwitch.ledState(0);
    panTiltX.angle = 90;
    panTiltY.angle = 90;
    panTilt.updateAngles();

    delay(50);

    panTilt.detach();
    while(!mSwitch.switchState()){
      delay(50);

    }
    mSwitch.ledState(1);
    panTilt.begin();
    laser.fire(1);
    schedule.restart();
    #ifdef MAIN_DEBUG
      Serial.print(F("Switch is on!"));
    #endif
  }

  panTiltX.angle = getDeltaPosition(&panTiltX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltX.angle;
  panTiltY.angle = getDeltaPosition(&panTiltY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTiltY.angle;
  //Serial.println(F("Update Angles"));
  panTilt.updateAngles();


  if(markovShakeState == 2){
    shake();
  }


  laser.fire(1);
  delay(5);
}


int getMarkovDirection(panTiltPos_t *pt, int changeProb){

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


int getDeltaPosition(panTiltPos_t *pt, int funcChangeVal, int changeProb){
  int tempVal = getMarkovDirection(pt, changeProb);

  tempVal *= funcChangeVal;

  return tempVal;

}


int markovPause(){
  int val = random(101);

  if(val < 25){
    return random(1500, 2000);
  }
  else if(val < 90){
    return random(1000, 1500);
  }
  else{
    return random(500, 750);
  }
}

void sleep(unsigned long minSec, unsigned long maxSec){
  unsigned int delayVal = random(minSec, maxSec);
  laser.fire(0);
  panTilt.detach();

  unsigned long startTime = millis();
  for(unsigned long i = 0; i < delayVal; i++){
    while(millis() - startTime < 1000){
      heartBeat(10000);

      if(!mSwitch.switchState()){ //return if switch is turned off. Main program waits for restart.
        return;
      }
      if(startTime > millis()){ //check for rollovers
        startTime = millis();
        break;

      }
    }
    startTime = millis();
  }
  panTilt.begin();
  laser.fire(1);

}


void heartBeat(int hbInterval){
  static unsigned long oldTime;

  if(millis() - oldTime > hbInterval){
    mSwitch.heartBeat(1);
    oldTime = millis();
  }
}

void shake(){
  int moveVal = 10;
  const int shakeDelay = 0;
  panTiltX.angle += moveVal;
  panTilt.updateAngles();
  delay(shakeDelay);
  panTiltX.angle -= 2*moveVal;
  panTilt.updateAngles();
  delay(shakeDelay);
  panTiltX.angle += moveVal;
  panTilt.updateAngles();
  delay(shakeDelay);
}
