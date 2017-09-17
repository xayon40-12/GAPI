//
// Created by Nathan Touroux on 17/09/2017.
//

#ifndef GAPI_P_HPP
#define GAPI_P_HPP

#include "Tools/Thread.hpp"

class P: public Thread{
private:
    int i;
public:
    int max = 100;
    P();

    void inc();

    void run() override;
};


#endif //GAPI_P_HPP
