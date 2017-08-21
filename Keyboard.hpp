//
//  Keyboard.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <iostream>
#include "Event.hpp"

class Keyboard : public Event{
private:
    static bool isKey(State state, SDL_Keycode key);
    
public:
    static bool isKeyPressed(SDL_Keycode key);
    static bool isKeyHeld(SDL_Keycode key);
    static bool isKeyReleased(SDL_Keycode key);
    static bool isKeyIdle(SDL_Keycode key);
};

#endif /* Keyboard_hpp */
