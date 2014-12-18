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


#define MIN_LOOP_TIME 0


int markovShakeState = 1;
int changeVal;

LinkedMarkov lmSpeed;
LinkedMarkov lmShake;
LinkedMarkov lmPause;

PanTilt panTilt( SERVO_X_PIN, SERVO_Y_PIN );


Task pauseTask(&pauseCB);
Task updateMarkovTask(&updateMarkov, 750, 1);
//Timer minLoopTime( MIN_LOOP_TIME, 1 );

void updateMarkov(){
  changeVal = lmSpeed.getNextValue();
  markovShakeState = lmShake.getNextValue();
  updateMarkovTask.enable();


}

//halt laser at certain spot for a few moments at this time
void setNextPauseTime(unsigned long avg_sec_to_pause=10, double variance=6){

  unsigned long temp = gauss.gRandom(avg_sec_to_pause, variance)*1000;

  #ifdef SERIAL_DEBUG
  #ifdef TIME_DEBUG
  MY_SERIAL.print(F("Next pause in "));
  MY_SERIAL.print(temp/1000);
  MY_SERIAL.println(F(" seconds.\n"));
  #endif
  #endif

  pauseTask.setInterval(temp);
  pauseTask.enable();

}


void pauseCB(){


  pauseTask.disable();
  panTilt.pause( markovPause() );

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
  lmShake.addLinkToBack( 1,  0, 5 ); // No shake
//                           ^  ||
//                           |  \/
  lmShake.addLinkToBack( 2, 30, 0 ); // Shake

  randomSeed(analogRead(5));


  scheduler.addEvent(&pauseTask);
  scheduler.addEvent(&updateMarkovTask);
  //scheduler.addEvent(&minLoopTime);


#ifdef SERIAL_DEBUG
  MY_SERIAL.println(F("setup complete"));
#endif

delay(5);

}


void loop(){


  #ifdef SERIAL_DEBUG
  MY_SERIAL.println(panTilt.getState());
  #endif


//if(minLoopTime.check(ELAPSE_RESTART)){
    if( panTilt.getState() == STATE_RUN ){

      panTilt.posX.angle = getDeltaPosition(&panTilt.posX, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posX.angle;
      panTilt.posY.angle = getDeltaPosition(&panTilt.posY, changeVal, DIRECTION_CHANGE_PROBABILITY) + panTilt.posY.angle;


      if(markovShakeState == 2){
        panTilt.shake();
      }
  }
//}

  scheduler.run();
  panTilt.update();
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
