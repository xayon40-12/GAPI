//
//  Keyboard.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Keyboard.hpp"

bool Keyboard::isKey(State state, SDL_Keycode key){
    auto it = keys.find(key);
    if(it != keys.end()){
        if(it->second == PRESSED && (state == HELD || state == PRESSED)) toUpdatekeys[key] = HELD;//TODO problem update should be done in event the next frame, use an array of keys to update
        else if(it->second == RELEASED && (state == IDLE || state == RELEASED)) toUpdatekeys[key] = IDLE;//TODO the same
        else if(it->second != state) return false;
        return true;
    }
    return false;
}
bool Keyboard::isKeyPressed(SDL_Keycode key){
    return isKey(PRESSED, key);
}
bool Keyboard::isKeyHeld(SDL_Keycode key){
    return isKey(HELD, key);
}
bool Keyboard::isKeyReleased(SDL_Keycode key){
    return isKey(RELEASED, key);
}
bool Keyboard::isKeyIdle(SDL_Keycode key){
    return isKey(IDLE, key);
}
