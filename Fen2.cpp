//
//  Fen2.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 06/07/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Fen2.hpp"


Fen2::Fen2(const std::string &title, int width, int height) : Window(title, width, height){
    
}

Fen2::~Fen2() = default;


void Fen2::paintComponent(){
    setColour(Colour::white());
    clear();
    
    if(isActive()){
        setColour(Colour::red());
        fillRect(Mouse::x()-50, Mouse::y()-50, 100, 100);
    }
}

void Fen2::run(){
    if(!isClosed()){
        if(Keyboard::isKeyHeld(SDLK_ESCAPE)) close();
        
        updateInput();
        
        
        present();
    }
}
