
/**************************************************************************/
/*!
    @file     stuMarkov.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for handling my Markov probabilities.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#include "stuMarkov.h"

LinkedMarkov::LinkedMarkov(){


}

void LinkedMarkov::begin(){
  _itr = 0 ;
  _head = &_mSpeed[ _itr ] ;
  _current = _head ;


}

void LinkedMarkov::addLink( unsigned int speed, unsigned int prevVal, unsigned int nextVal ) {

  assert( prevVal + nextVal <= 100 );             //make sure that change probabilites are less than one hundred

  markovSpeed_t* current = &_mSpeed[ _itr++ ] ;   //get pointer to current spot in linked list and iterate one spot.

  current->next.markovSpeed = &_mSpeed[ _itr ] ;  //set current next to next spot
  markovSpeed_t* previous = current ;             //get placeholder to old current
  current = &_mSpeed[ _itr ] ;                    //set current pointer to next spot

  current->speed = speed ;

  current->next.markovSpeed = _head ;             // new last element, so set next to head.
  current->next.probability = nextVal ;

  current->previous.markovSpeed = previous ;
  current->previous.probability = prevVal ;

}

unsigned  int LinkedMarkov::getNextSpeed( int randVal ){

  assert( randVal >= 0 ) ;
  assert( randVal <= 100 ) ;

  if( randVal < _current->previous.probability ){
    _current = _current->previous.markovSpeed ;
  }
  else if( randVal < _current->previous.probability + _current->next.probability ){
    _current = _current->next.markovSpeed ;

  }

  return _current->speed ;

}
