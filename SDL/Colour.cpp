//
//  Colour.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Colour.hpp"
Colour Colour::black() { return Colour(0, 0, 0); }
Colour Colour::white() { return Colour(255, 255, 255); }
Colour Colour::gray() { return Colour(127, 127, 127); }
Colour Colour::red() { return Colour(255, 0, 0); }
Colour Colour::green() { return Colour(0, 255, 0); }
Colour Colour::blue() { return Colour(0, 0, 255); }
Colour Colour::orange() { return Colour(255, 127, 0); }
Colour Colour::purple() { return Colour(120, 0, 128); }
Colour Colour::cyan() { return Colour(0, 255, 255); }
Colour Colour::yellow() { return Colour(255, 255, 0); }
Colour Colour::magenta() { return Colour(255, 0, 255); }
Colour Colour::brown() { return Colour(150, 75, 0); }

Colour::Colour() : r(0), g(0), b(0), a(255){
    
}
Colour::Colour(Colour const &c) = default;

Colour::Colour(u_char r, u_char g, u_char b) : r(r), g(g), b(b), a(255){
    
}
Colour::Colour(u_char r, u_char g, u_char b, u_char a) : r(r), g(g), b(b), a(a){
    
}
Colour::~Colour() = default;
