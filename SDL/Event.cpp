//
//  Event.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Event.hpp"
#include "Window.hpp"

int Event::activeWindow = 0;
std::unordered_map<Uint32, Window*> Event::windows;

bool Event::autoUpdate = false;

SDL_Event Event::event;
Uint8 Event::buttons[20];
std::unordered_map<Sint32, Uint8> Event::toUpdateButtons;
int Event::maxButtons = sizeof(Event::buttons)/sizeof(Uint8);
std::unordered_map<Sint32, Uint8> Event::keys;
std::unordered_map<Sint32, Uint8> Event::toUpdatekeys;

int Event::m_x, Event::m_y, Event::m_xrel, Event::m_yrel, Event::m_xOnScreen, Event::m_yOnScreen, Event::m_xWheel = 0, Event::m_yWheel = 0;

void Event::initWindow(){
    SDL_Init(SDL_INIT_VIDEO);
}
void Event::endWindow(){
    SDL_Quit();
}

void Event::update(){
    m_xrel = 0;
    m_yrel = 0;
    while(SDL_PollEvent(&event)){
        if(event.window.event == SDL_WINDOWEVENT_CLOSE){
            if(windows[event.window.windowID]->operation == EXIT_ON_CLOSE){
                windows[event.window.windowID]->close();
            }
        }
        if(event.window.event == SDL_WINDOWEVENT_ENTER){
            activeWindow = event.window.windowID;
        }

        for(auto &k: toUpdatekeys){//updating the chages from pressed to held and released to idle for keys
            keys[k.first] = k.second;
        }
        toUpdatekeys.clear();//clear the changes
        if(event.type == SDL_KEYDOWN) if(event.key.repeat == 0) keys[event.key.keysym.sym] = PRESSED;
        if(event.type == SDL_KEYUP) if(event.key.repeat == 0) keys[event.key.keysym.sym] = RELEASED;

        for(auto &b: toUpdateButtons){//updating the chages from pressed to held and released to idle for buttons
            buttons[b.first] = b.second;
        }
        toUpdateButtons.clear();//clear the changes
        if(event.type == SDL_MOUSEBUTTONDOWN) if(event.button.button < maxButtons) buttons[event.button.button] = PRESSED;
        if(event.type == SDL_MOUSEBUTTONUP) if(event.button.button < maxButtons) buttons[event.button.button] = RELEASED;
        
        if(event.type == SDL_MOUSEMOTION){
            m_x = event.motion.x;
            m_xrel = event.motion.xrel;
            m_y = event.motion.y;
            m_yrel = event.motion.yrel;
        }
        if(event.type == SDL_MOUSEWHEEL){
            m_xWheel = event.wheel.x;
            m_yWheel = event.wheel.y;
        }
    }
    SDL_GetGlobalMouseState(&m_xOnScreen, &m_yOnScreen);
}

Event::Event(): operation(EXIT_ON_CLOSE) {

}

Event::~Event() = default;
