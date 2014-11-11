
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

#include "Arduino.h"
#include <assert.h>

struct markovSpeed_t;

typedef struct markovPair_t{

          unsigned  int       probability;
          markovSpeed_t*      markovSpeed;

}markovPair_t;


typedef struct markovSpeed_t{

          unsigned  int      speed;


          markovPair_t       previous;
          markovPair_t       next;

}markovSpeed_t;




class LinkedMarkov {

public:
                    LinkedMarkov();

            void        begin();
            void        addLink( unsigned int speed, unsigned int prevVal, unsigned int nextVal ) ;

  unsigned  int     getNextSpeed( int randVal ) ;


private:

        markovSpeed_t     _mSpeed[ 5 ] ;
        markovSpeed_t*    _head ;
        markovSpeed_t*    _current ;
        uint8_t           _itr ;

};


#endif
