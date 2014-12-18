#pragma once/*<=DON'T TOUCH!!!!!!!*/
/**************************************************************************/
/*!                   ****Customizable Settings****

Use this file to change the laser settings (e.g. the amount of time the
laser runs before shutting off in sleep mode).


                      ****EDIT THIS FILE ONLY****
/**************************************************************************/





/**************************************************************************/
/***********************Continuous Mode Settings***************************/
/************************* (ON => forever) ********************************/
/**************************************************************************/
/*                    (General purpose. Always on.)                       */
/* Settings:                                                              */
/*                                NONE                                    */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/***********************INTERMITTENT Mode Settings*************************/
/********************* (ON => OFF => ON => REPEAT) ************************/
/**************************************************************************/
/*               (Good for when the kitty is home alone)                  */
/*SETTINGS:                                                               */
/*                                                                        */
/* MINUTES the laser stays ON in intermittent mode for each cycle.        */
/*------------------------------------------------------------------------*/
  #define INTERMITTENT_ON_TIME   15 /*<=========== CHANGE ME (Default=15) */
/*------------------------------------------------------------------------*/
/*                                                                        */
/* MINUTES the laser stays OFF in intermittent mode for each cycle.       */
/*------------------------------------------------------------------------*/
  #define INTERMITTENT_OFF_TIME  60 /*<=========== CHANGE ME (Default=60) */
/*------------------------------------------------------------------------*/
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/***************************SLEEP Mode Settings****************************/
/************************ (ON => OFF => FOREVER) **************************/
/**************************************************************************/
/*       (Good for distracting a kitty when you want to go to sleep)      */
/* SETTINGS:                                                              */
/*                                                                        */
/* MINUTES the laser stays ON before turning OFF FOREVER.                 */
/* (You must change mode with dial to restart laser.)                     */
/*------------------------------------------------------------------------*/
  #define MINUTES_BEFORE_SLEEP    30 /*<=========== CHANGE ME (Default=30)*/
/*------------------------------------------------------------------------*/
/*                                                                        */
/**************************************************************************/



#define SERVO_MIN_X_AXIS 20
#define SERVO_MAX_X_AXIS 120

#define SERVO_MIN_Y_AXIS    110
#define SERVO_MAX_Y_AXIS    160


#define LASER_MIDPOINT_OFFSET_X 0
#define LASER_MIDPOINT_OFFSET_Y 0
#define LASER_PROBABILITY_X     0
#define LASER_PROBABILITY_Y     0
