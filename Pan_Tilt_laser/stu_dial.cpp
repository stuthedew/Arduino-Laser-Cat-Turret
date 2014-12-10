/**************************************************************************/
/*!
@file     stu_dial.cpp
@author   Stuart Feichtinger
@license  MIT (see license.txt)

Library to read input from potentiometer and return appropriate run mode.


@section  HISTORY
v0.0.1 - First release

*/
/**************************************************************************/

#include "stu_dial.h"

/*
#define LOWPASS_ANALOG_PIN_AMT 6
[LOWPASS_ANALOG_PIN_AMT],
lowpass_cur_out[LOWPASS_ANALOG_PIN_AMT];
int        lowpass_input[LOWPASS_ANALOG_PIN_AMT];

*/

int adcsample_and_lowpass(int pin, int sample_rate, int samples, float alpha, char use_previous) {
  // pin:            arduino analog pin number to sample on   (should be < LOWPASS_ANALOG_PIN_AMT)
  // sample_rate:    approximate rate to sample at (less than ~9000 for default ADC settings)
  // samples:        how many samples to take in this call
  // alpha:          lowpass alpha
  // use_previous:   If true, we continue adjusting from the most recent output value.
  //                If false, we do one extra analogRead here to prime the value.
  //   On noisy signals this non-priming value can be misleading,
  //     and with few samples per call it may not quite adjust to a realistic value.
  //   If you want to continue with the value we saw last -- which is most valid when the
  //     value is not expected to change significantly between calls, you can use true.
  //   You may still want one initial sampling, possibly in setup(), to start from something real.

  static float
    lowpass_prev_out,
    lowpass_cur_out;

  static int
  lowpass_input;

  float one_minus_alpha = 1.0-alpha;
  int micro_delay = max(100, (1000000/sample_rate) - 160); //with 160 being our estimate of how long a loop takes
  //(~110 for analogRead at the default ~9ksample/sec,  +50 grasped from thin air (TODO: test)
  if (!use_previous) {
    //prime with a real value (instead of letting it adjust from the value in the arrays)
    lowpass_input = analogRead(pin);
    lowpass_prev_out = lowpass_input;
  }

  //Do the amount of samples, and lowpass along the way
  int i;
  for (i=samples;i>0;i--) {
    delayMicroseconds(micro_delay);
    lowpass_input = analogRead(pin);
    lowpass_cur_out = alpha*lowpass_input + one_minus_alpha*lowpass_prev_out;
    lowpass_prev_out=lowpass_cur_out;
  }
  return lowpass_cur_out;
}




  StuDial Dial;


void StuDial::setPin( uint8_t dialPin ){
  _dialPin = dialPin ;

}

void StuDial::begin( void ){
  pinMode( _dialPin, INPUT ) ;
  update() ;

}

void StuDial::update( void ){
  int adcReading = 0;

    analogReference(INTERNAL); //set analog reference to internal 1.1V
    //adcReading = analogRead( _dialPin ) * 1100 ; //scale to adjust for Aref
    adcReading = adcsample_and_lowpass( _dialPin ) * 1100 ; //scale to adjust for Aref
    adcReading += 128; // Adjust for rounding when bitshifting right.
    adcReading >>= 10; // divide by 1024
    analogReference(DEFAULT);


#ifndef EMBED
  Serial.println(F("Reading:"));
  Serial.println(adcReading);
#endif

  if( abs(adcReading - MAX_OFF_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_OFF ;

  }else if( abs(adcReading - MAX_CONT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_CONTINUOUS ;
  }

  else if( abs(adcReading - MAX_INT_ADC) <= ADC_VALUE_RANGE ){
    _mode = MODE_INTERMITTENT ;

  }
  else
    {
      _mode = MODE_INTERMITTENT ;
  }

}

runmode_e StuDial::getMode( void ) const{
  return _mode ;
}
