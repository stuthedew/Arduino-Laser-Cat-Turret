
/**************************************************************************/
/*!
    @file     missileswitch.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Simple library for reading input from Sparkfun's missile switch
    (https://www.sparkfun.com/products/11310) and controlling its led.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/


#include "missileswitch.h"

Missileswitch::Missileswitch(int switchPin, int ledPin):_ledPin(ledPin), _switchPin(switchPin){
  _switchState = 0;
  _ledState = 0;

}

void Missileswitch::begin(){
  pinMode(_ledPin, OUTPUT);
  pinMode(_switchPin, INPUT);

  ledState(1);

}

void Missileswitch::ledState(bool state){
  _ledState = state;

  digitalWrite(_ledPin, !_ledState);

}

bool Missileswitch::ledState() const {

  return _ledState;
}

void Missileswitch::heartBeat(int numHeartBeat){
  bool state = _ledState;
  for(int i = 0; i < numHeartBeat*2; i++){
    state ^= 1;
    ledState(state);
    delay(100);
  }
}


bool Missileswitch::switchState(){

  return _debounce();;
}


bool Missileswitch::_debounce(){
  const int debounceDelay = 50;
  const unsigned int timeOutDelay = 500;

  unsigned long lastDebounceTime, processTime;
  bool oldReading;
  processTime = millis();
  lastDebounceTime = millis();

  while(timeOutDelay > millis() - processTime){
    bool reading = digitalRead(_switchPin);
    if(reading != oldReading){
      oldReading = reading;
      lastDebounceTime = millis();
    }
    else if((millis() - lastDebounceTime) > debounceDelay){
      return reading;
    }
  }
  Serial.println(F("Switch Read TIMEOUT!!!!"));
  return _switchState;
}
