//
// Created by Nathan Touroux on 17/09/2017.
//

#include "P.hpp"

P::P(){

}

void P::inc(){
    lockMutex("i");
    i++;
    sleepMillis(10);
    unlockMutex("i");
}

void P::run(){
    i = 0;
    while(i<max){
        lockMutex("i");
        std::cout << "i = " << i << std::endl;
        unlockMutex("i");
    }
}