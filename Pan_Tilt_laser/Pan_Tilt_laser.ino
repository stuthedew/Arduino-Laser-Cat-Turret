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
*/
/**************************************************************************/

#include "panTilt_config.h"
#include "stuPanTilt.h"
#include <Time.h>
#include <Servo.h>
#include "stuMarkov.h"
#include "stuLaser.h"
#include "stu_scheduler.h"
#include "stu_gauss.h"
#include <Gaussian.h>
#include "stu_dial.h"


#define EMBED

int markovShakeState = 1;
int changeVal;

LinkedMarkov lmSpeed;
LinkedMarkov lmShake;
LinkedMarkov lmPause;



PanTilt panTilt(SERVO_X_PIN, SERVO_Y_PIN );




void updateMarkov( void ){
  changeVal = lmSpeed.getNextValue();
  markovShakeState = lmShake.getNextValue();

}



Task pauseTask(&pauseCB);
Task restTask(&restCB);
Task sleepTask(&sleepCB);
Task speedAndDirTask(&updateSpeedAndDir, 750);

//StuScheduler schedule;

void updateSpeedAndDir(){
  changeVal = lmSpeed.getNextValue();
  markovShakeState = lmShake.getNextValue();

}

//halt laser at certain spot for a few moments at this time
void setNextPauseTime(unsigned long avg_sec_to_pause=10, double variance=6){

  unsigned long temp = gauss.gRandom(avg_sec_to_pause, variance)*1000;

  #ifdef TIME_DEBUG
  Serial.print(F("Next pause in "));
  Serial.print(temp/1000);
  Serial.println(F(" seconds.\n"));
  #endif

  pauseTask.setInterval(temp);

}

//turn off laser for a few moments at this time
void setNextRestTime(unsigned long avg_sec_to_rest=360, double variance=60){
  unsigned long temp = gauss.gRandom( avg_sec_to_rest, variance ) * 1000 ;

  #ifdef TIME_DEBUG
  Serial.print(F("Next rest in "));
  Serial.print(temp/1000);
  Serial.println(F(" seconds.\n"));
  #endif

  restTask.setInterval(temp);
}


//turn of laser for a minutes to hours at this time
void setNextSleepTime(unsigned long avg_min_to_sleep=10, double variance = 3){
  unsigned long mSecToSleep = max(1, gauss.gRandom(avg_min_to_sleep, variance))*60000;

  //  unsigned long mSecToSleep = gauss.gRandom(avg_min_to_sleep, variance)*60000;
  #ifdef TIME_DEBUG
  Serial.print(F("Next sleep in "));
  Serial.print(mSecToSleep/1000);
  Serial.println(F(" seconds.\n"));
  #endif

  sleepTask.setInterval(mSecToSleep);
}


void pauseCB(){
  //Serial.println(F("Pause Callback!"));
  delay(markovPause());
  setNextPauseTime();
}

void restCB(){
  //Serial.println(F("Rest Callback!"));

  //  mSwitch.ledState(0);  TODO: Replace missile switch
  //panTilt.setMode(MODE_REST);
  //sleep(5, 10);
  //  mSwitch.ledState(1);  TODO: Replace missile switch
  setNextPauseTime();
  setNextRestTime();
}

void sleepCB(){
  //Serial.println(F("Sleep Callback!"));
  //panTilt.setMode(MODE_SLEEP);
  //sleep(1800, 2400); //sleep between 30 and 40 minutes
  //panTilt.setMode(MODE_INTERMITTENT);
  setNextPauseTime();
  setNextRestTime();
  setNextSleepTime();
}

void setup() {

  #ifndef EMBED
    Serial.begin(BAUD_RATE);
    Serial.println(F("setup starting..."));
  #endif


  panTilt.begin();



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


  //        addLinkToBack(state, previous_state_probability, next_state_probability)
  lmPause.addLinkToBack( 1,  0, 5 ); // No Pause
  //                         ^  ||
  //                         |  \/
  lmPause.addLinkToBack( 2, 40, 60 ); // Pause
  //                         ^  ||
  //                         |  \/
  lmPause.addLinkToBack( 1, 30, 0 ); // No Pause
  //                        ^  ||
  //                        |  \/
  //                     | No Pause |

  randomSeed(analogRead(5));



#ifndef EMBED
  Serial.println(F("setup complete"));
#endif

}


void loop() {

  panTilt.update();

  delay( 50 );
  return; //  TODO: Remove

if( panTilt.getState() != STATE_RUN ){
  delay( 100 );
  return;
}

  panTilt.posX.angle = getDeltaPosition(&panTilt.posX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posX.angle;
  panTilt.posY.angle = getDeltaPosition(&panTilt.posY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posY.angle;

  panTilt.update();


  if(markovShakeState == 2){
    panTilt.shake();
  }

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
