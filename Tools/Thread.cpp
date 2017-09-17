//
//  Thread.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 04/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Thread.hpp"

Thread::Thread(): isstop(true), thread(nullptr), autoDelete(false)
{
    
}
Thread::~Thread() = default;

bool Thread::start(const std::string &name)
{
    if(isstop)
    {
        thread = new std::thread(Thread::ThreadInit, this);
        if(thread != nullptr)
        {
            isstop = false;
            return true;
        }
    }
    return false;
}
void Thread::join()
{
    thread->join();
    delete thread;
    isstop = true;
}
void Thread::setAutoDelete(bool autoDelete)
{
    autoDelete = autoDelete;
}

int Thread::ThreadInit(void* param)
{
    Thread *t(reinterpret_cast<Thread*>(param));
    t->run();
    if(t->autoDelete)
        delete t;
    return 0;
}

bool Thread::threadRunning()
{
    return isstop;
}

void Thread::lockMutex(const std::string &name)
{
    mutexs[name].lock();
}
bool Thread::unlockMutex(const std::string &name)
{
    auto mutex = mutexs.find(name);
    
    if(mutexs.find(name) == mutexs.end())
        return false;
    else
        mutex->second.unlock();
    
    return true;
}

void Thread::sleep(long long t){
    std::this_thread::sleep_for(std::chrono::seconds(t));
}
void Thread::sleepMillis(long long t){
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}
void Thread::sleepMicro(long long t){
    std::this_thread::sleep_for(std::chrono::microseconds(t));
}
