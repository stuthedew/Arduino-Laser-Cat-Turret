
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
  _tail = _head ;
  _current->previous.markovLink = _head ;
  _current->next.markovLink = _head ;


}

void LinkedMarkov::addLink( unsigned int speed, unsigned int prevVal, unsigned int nextVal ) {

  assert( prevVal + nextVal <= 100 );             //make sure that change probabilites are less than one hundred
  assert( _itr < LINKED_LIST_SIZE ) ;             // prevent overflow.

  _current = &_mSpeed[ _itr++ ] ;                 //get pointer to current spot in linked list and iterate pointer.

  _current->speed = speed ;

  _current->next.markovLink = _head ;             // new last element (tail), so set next to head.
  _current->next.probability = nextVal ;

  _current->previous.markovLink = _tail ;         // set current (new tail) previous-link to old tail
  _current->previous.probability = prevVal ;

  _tail->next.markovLink = _current ;             // set old tail next-link to current (new tail)
  _tail = _current ;                              // set tail pointer to new tail (current)

}

unsigned int LinkedMarkov::getNextSpeed( uint8_t randVal ) {

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

int LinkedMarkov::getListSize( void ) const {
    return _itr;

}


markovLink_t* LinkedMarkov::getMarkovPtr( uint8_t Pos ) {
    assert( Pos < _itr );
    markovLink_t* temp = &_mSpeed[ Pos ] ;

    return  temp;

}
