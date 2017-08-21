//
//  Fen2.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 06/07/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Fen2_hpp
#define Fen2_hpp

#include "Window.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

class Fen2 : public Window{
private:
    
public:
    Fen2(std::string title, int width, int height);
    ~Fen2();
    
    void paintComponent();
    
    void run();
};

#endif /* Fen2_hpp */
