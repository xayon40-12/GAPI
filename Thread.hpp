//
//  Thread.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 04/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Thread_hpp
#define Thread_hpp

#include <iostream>
#include <map>
#include <thread>

class Thread{
private:
    static int ThreadInit(void* param);
    bool isstop;
    std::thread *thread;
    bool autoDelete;
    std::map<std::string, std::mutex> mutexs;
    
public:
    Thread();
    virtual ~Thread();
    bool start(std::string name = "");// start run() in a nex thread
    void join();// wait run() to finish
    void setAutoDelete(bool autoDelete);//if your threaded class is a pointer it will be auto delete if you set autodelete to true
    
    bool threadRunning();
    
    void lockMutex(std::string name);
    bool unlockMutex(std::string name);
    
    static void sleep(long long t);
    static void sleepMillis(long long t);
    static void sleepMicro(long long t);
    
    virtual void run() = 0;// tout devra se passer ici
};

#endif /* Thread_hpp */
