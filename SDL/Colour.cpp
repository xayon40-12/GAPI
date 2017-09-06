//
//  Colour.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Colour.hpp"
Colour Colour::black(0, 0, 0);
Colour Colour::white(255, 255, 255);
Colour Colour::gray(127, 127, 127);
Colour Colour::red(255, 0, 0);
Colour Colour::green(0, 255, 0);
Colour Colour::blue(0, 0, 255);
Colour Colour::orange(255, 127, 0);
Colour Colour::purple(120, 0, 128);
Colour Colour::cyan(0, 255, 255);
Colour Colour::yellow(255, 255, 0);
Colour Colour::magenta(255, 0, 255);
Colour Colour::brown(150, 75, 0);

Colour::Colour() : r(0), g(0), b(0), a(255){
    
}
Colour::Colour(Colour const &c) : r(c.r), g(c.g), b(c.b), a(c.a){
    
}
Colour::Colour(u_char r, u_char g, u_char b) : r(r), g(g), b(b), a(255){
    
}
Colour::Colour(u_char r, u_char g, u_char b, u_char a) : r(r), g(g), b(b), a(a){
    
}
Colour::~Colour(){
    
}
