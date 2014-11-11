
/**************************************************************************/
/*!
    @file     stuMarkov.h
    @author   Stuart Feichtinger
    @license  MIT (see license.txt)

    Library for handling my Markov probabilities.


    @section  HISTORY
    v1.0 - First release

*/
/**************************************************************************/

#ifndef _STUMARKOV_H_
#define _STUMARKOV_H_

#define LINKED_LIST_SIZE 5

#ifdef ARDUINO
#include "Arduino.h"
#endif

#include <assert.h>

struct markovLink_t;

typedef struct markovPair_t{

          unsigned  int       probability;
          markovLink_t*      markovLink;

}markovPair_t;


typedef struct markovLink_t{

          unsigned  int      speed;


          markovPair_t       previous;
          markovPair_t       next;

}markovLink_t;




class LinkedMarkov {

public:
                           LinkedMarkov( void ) ;

            void           begin( void ) ;
            void           addLink( unsigned int speed, unsigned int prevVal, unsigned int nextVal ) ;

  unsigned  int            getNextSpeed( uint8_t randVal ) ;
            int            getListSize( void ) const ;

 markovLink_t*             getMarkovPtr( uint8_t relativePos ) ;


private:

        markovLink_t     _mSpeed[ LINKED_LIST_SIZE ] ;
        markovLink_t*    _head ;
        markovLink_t*    _tail ;
        markovLink_t*    _current ;

        int8_t           _itr ;

};


#endif
