//
//  Mouse.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Mouse.hpp"

bool Mouse::isButton(State state, int button){
    internalUpdate();
    if(button >= 0 && button < maxButtons){
        if(buttons[button] == PRESSED && (state == HELD || state == PRESSED)) buttons[button] = HELD;
        else if(buttons[button] == RELEASED && (state == IDLE || state == RELEASED)) buttons[button] = IDLE;
        else if(buttons[button] != state) return false;
        return true;
    }
    return false;
}
bool Mouse::isButtonPressed(int button){
    return isButton(PRESSED, button);
}
bool Mouse::isButtonHeld(int button){
    return isButton(HELD, button);
}
bool Mouse::isButtonReleased(int button){
    return isButton(RELEASED, button);
}
bool Mouse::isButtonIdle(int button){
    return isButton(IDLE, button);
}

int Mouse::x(){
    return m_x;
}
int Mouse::y(){
    return m_y;
}
int Mouse::xRelative(){
    return m_xrel;
}
int Mouse::yRelative(){
    return m_yrel;
}
int Mouse::xOnScreen(){
    return m_xOnScreen;
}
int Mouse::yOnScreen(){
    return m_yOnScreen;
}
int Mouse::xWheel(){
    return m_xWheel;
}
int Mouse::yWheel(){
    return m_yWheel;
}

bool Mouse::moved(){
    return (m_xrel != 0 || m_yrel != 0);
}

void Mouse::showPointer(bool enable){
    SDL_ShowCursor(enable?SDL_TRUE:SDL_FALSE);
}
void Mouse::capturePointer(bool enable){
    SDL_SetRelativeMouseMode(enable?SDL_TRUE:SDL_FALSE);
}
void Mouse::captureMouse(bool enable){
    SDL_CaptureMouse(enable?SDL_TRUE:SDL_FALSE);
}
