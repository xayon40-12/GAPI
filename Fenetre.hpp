//
//  Fenetre.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Fenetre_hpp
#define Fenetre_hpp

#include <iostream>
#include "GLWindow.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Frustum.hpp"

class Fenetre : public GLWindow{
private:
    FrameBuffer frameBuffer;
    Camera camera;
    Object obj, obj2;
    
    glm::mat4 perspective;
    
    int option;
public:
    Fenetre(std::string title, int width, int height, int major = 2, int minor = 1);
    ~Fenetre();
    
    void paintComponent();
    
    void run();
};

#endif /* Fenetre_hpp */
