//
//  Event.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include <iostream>
#include <SDL.h>
#include <map>
#include <vector>
#include <unordered_map>

class Window;

enum CloseOperation {EXIT_ON_CLOSE = 12, DO_NOTHING_ON_CLOSE};
enum State {IDLE, RELEASED, PRESSED, HELD};

class Event{
private:
    static bool autoUpdate;
    static SDL_Event event;
protected:
    CloseOperation operation;
    static std::unordered_map<Uint32, Window*> windows;
    static int activeWindow;
    
    static int m_x, m_y, m_xrel, m_yrel, m_xOnScreen, m_yOnScreen, m_xWheel, m_yWheel;
    static Uint8 buttons[20];
    static std::unordered_map<Sint32, Uint8> toUpdateButtons;// TODO use toUpdateButtons for the passage from pressed to hold and update after
    static int maxButtons;
    static std::unordered_map<Sint32, Uint8> keys;//TODO change to normal array
    static std::unordered_map<Sint32, Uint8> toUpdatekeys;
    
    static void internalUpdate();
    
public:
    Event();
    virtual ~Event();
    
    static void initWindow();
    static void endWindow();
    
    static void update();
    static void setAutoUpdate(bool autoUpdate);
};

#endif /* Event_hpp */
