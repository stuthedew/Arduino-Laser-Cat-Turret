
/**************************************************************************/
/*!
    @file     stuMarkov.cpp
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for handling my Markov probabilities.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/

#include "stuMarkov.h"

namespace stu{


  LinkedMarkov::LinkedMarkov(){
    _itr = 0 ;
    _head = &_mValue[ _itr ] ;
    _current = _head ;
    _tail = _head ;
    _current->previous.markovLink = _head ;
    _current->next.markovLink = _head ;

  }


  void LinkedMarkov::addLinkToBack( unsigned int value, unsigned int prevVal, unsigned int nextVal ) {

    assert( prevVal + nextVal <= 100 );       // make sure that change probabilites are less than one hundred
    assert( _itr < LINKED_LIST_SIZE ) ;       // prevent overflow.

    markovLink_t* newLinkPtr;

    newLinkPtr = &_mValue[ _itr++ ] ;         // get pointer to current spot in linked list and iterate pointer for next time.

    newLinkPtr->value = value ;

    newLinkPtr->next.markovLink = _head ;     // new last element (tail), so set next to head.
    newLinkPtr->next.probability = nextVal ;

    newLinkPtr->previous.markovLink = _tail ; // set new link (new tail) previous-link to old tail
    newLinkPtr->previous.probability = prevVal ;

    _tail->next.markovLink = newLinkPtr ;     // set old tail next-link to new link (new tail)
    _tail = newLinkPtr ;                      // set tail pointer to new tail (new link)

  }

  unsigned int LinkedMarkov::getNextValue( void ) {
    uint8_t randVal = random( 101 ) ;
    assert( randVal >= 0 ) ;
    assert( randVal <= 100 ) ;

    if( randVal < _current->previous.probability ){
      _current = _current->previous.markovLink ;
    }
    else if( randVal < _current->previous.probability + _current->next.probability ){
      _current = _current->next.markovLink ;
    }

    return _current->value ;

  }

  int LinkedMarkov::getListSize( void ) const {
      return _itr ;
  }


  markovLink_t* LinkedMarkov::getMarkovPtr( uint8_t Pos ) {
      assert( Pos < _itr ) ;
      markovLink_t* temp = &_mValue[ Pos ] ;

      return  temp ;

  }

}// end namespace
