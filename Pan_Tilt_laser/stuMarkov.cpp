
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
  _current->previous.markovLink = _head ;
  _current->next.markovLink = _head ;


}

void LinkedMarkov::addLink( unsigned int speed, unsigned int prevVal, unsigned int nextVal ) {

  assert( prevVal + nextVal <= 100 );             //make sure that change probabilites are less than one hundred

  markovLink_t* oldLink = &_mSpeed[ _itr++ ] ;   //get pointer to current spot in linked list and iterate one spot.

  markovLink_t* current = &_mSpeed[ _itr ] ;   //get pointer to new current spot in linked list.

  current->speed = speed ;

  current->next.markovLink = _head ;             // new last element, so set next to head.
  current->next.probability = nextVal ;

  current->previous.markovLink = oldLink ;
  current->previous.probability = prevVal ;
  oldLink->next.markovLink = current ;

}

unsigned  int LinkedMarkov::getNextSpeed( int randVal ){

  assert( randVal >= 0 ) ;
  assert( randVal <= 100 ) ;

  if( randVal < _current->previous.probability ){
    _current = _current->previous.markovLink ;
  }
  else if( randVal < _current->previous.probability + _current->next.probability ){
    _current = _current->next.markovLink ;

  }

  return _current->speed ;

}
