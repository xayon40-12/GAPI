//
//  Mouse.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Mouse_hpp
#define Mouse_hpp

#include <iostream>
#include "Event.hpp"

class Mouse : public Event{
private:
    static bool isButton(State state, int button);
    
public:
    static bool isButtonPressed(int button);
    static bool isButtonHeld(int button);
    static bool isButtonReleased(int button);
    static bool isButtonIdle(int button);
    
    static int x();
    static int y();
    static int xRelative();
    static int yRelative();
    static int xOnScreen();
    static int yOnScreen();
    static int xWheel();
    static int yWheel();
    
    static bool moved();
    
    static void showPointer(bool enable);
    static void capturePointer(bool enable);
    static void captureMouse(bool enable);
};

#endif /* Mouse_hpp */
