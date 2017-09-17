//
//  Colour.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Colour_hpp
#define Colour_hpp

#include <iostream>
#include <vector>

struct Colour{
    u_char r, g, b, a;
    
    Colour();
    Colour(Colour const &c);
    Colour(u_char r, u_char g, u_char b);
    Colour(u_char r, u_char g, u_char b, u_char a);
    ~Colour();
    
    static Colour black();
    static Colour white();
    static Colour gray();
    static Colour red();
    static Colour green();
    static Colour blue();
    static Colour orange();
    static Colour purple();
    static Colour cyan();
    static Colour yellow();
    static Colour magenta();
    static Colour brown();
};

#endif /* Colour_hpp */
