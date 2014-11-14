
/**************************************************************************/
/*!
    @file     stuMarkov.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for handling my Markov probabilities.


    @section  HISTORY
    v0.0.1 - First release

*/
/**************************************************************************/
#pragma once

#define LINKED_LIST_SIZE 5

#ifdef ARDUINO
#include "Arduino.h"
#endif

#include <assert.h>

namespace stu{

struct markovLink_t;

typedef struct markovPair_t{

          unsigned  int       probability;
          markovLink_t*       markovLink;

}markovPair_t;


typedef struct markovLink_t{

          unsigned  int      value;


          markovPair_t       previous;
          markovPair_t       next;

}markovLink_t;




class LinkedMarkov {

public:
                           LinkedMarkov( void ) ;

            void           begin( void ) ;
            void           addLinkToBack( unsigned int value, unsigned int prevVal, unsigned int nextVal ) ;

  unsigned  int            getNextValue( void ) ;
            int            getListSize( void ) const ;

 markovLink_t*             getMarkovPtr( uint8_t relativePos ) ;


private:

        markovLink_t     _mValue[ LINKED_LIST_SIZE ] ;
        markovLink_t*    _head ;
        markovLink_t*    _tail ;
        markovLink_t*    _current ;

        int8_t           _itr ;

};

}// end namespace
