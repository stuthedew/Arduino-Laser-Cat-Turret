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
#include "stu_vector.h"
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


void setup() {
  Serial.begin(BAUD_RATE);
  panTilt.begin();
  laser.setOrigin(1, 38);
  laser.setPosition(13, 200);
  //Serial.println(laser.vX());
  //Serial.println(laser.vY());

  Serial.println(laser.vAngle());

}

void loop() {

}
