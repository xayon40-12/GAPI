//
//  Fenetre.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 24/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Fenetre.hpp"

#ifdef __APPLE__
#include "ResourcePath.hpp"
#else
#define resourcePath() std::string()
#endif

#ifdef DEBUG
#define debugPath std::string("../")
#else
#define debugPath std::string("")
#endif

#define shadersPath resourcePath()+debugPath+"resources/shaders/"
#define imagesPath resourcePath()+debugPath+"resources/images/"

Fenetre::Fenetre(std::string title, int width, int height, int major, int minor):
    GLWindow(title, width, height, true, major, minor),
    frameBuffer(width, height, 2),

obj(Object::createCube(glm::vec3(0, 0, 0), glm::vec3(0.4, 0, 0), glm::vec3(0, 0.4, 0), glm::vec3(0, 0, 0.4), shadersPath+"texture.vert", "", shadersPath+"texture.frag", {0.5, 0, 0}, imagesPath+"earth.jpg", imagesPath+"endN.png")),

    obj2(Object::createSquare(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), shadersPath+"final.vert", "", shadersPath+"final.frag")),
    perspective(glm::perspective<float>(glm::radians(70.0f), ((float)getWidth())/getHeight(), 0.01, 100)),
    option(1)
{
    frameBuffer.create();
    
    obj.create();
    obj2.create();
    
    camera.setPosition(glm::vec3(0, 0, 15));
    camera.setSpeed(glm::vec3(0.1, 0.1, 0.1));
    camera.setRotationSpeed(glm::vec3(0.01, 0.01, 0.01));
    camera.lookTowardTarget(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    //camera.setVerticalFree(true);
    //camera.mapRotation(ROLL, SDLK_a, SDLK_e);
    //camera.attachPosition([]()->glm::vec3 {static float a = M_PI/2, d = 3; a+=0.02; return glm::vec3(d*std::cos(a), 0, d*std::sin(a));});
    //camera.attachTarget([]()->glm::vec3 {return glm::vec3(0, 0, 0);});
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}
Fenetre::~Fenetre(){
    
}

void Fenetre::paintComponent(){
    glm::mat4 V = camera.lookAt(), VP = perspective*V;
    Frustum frustum(VP);
    
    
    setColour(Colour::white);
    clear();
    
    frameBuffer.use();
    setColour(Colour::white);
    clear();
    
    obj.use();
    obj.uniformData("light", glm::vec3(10));
    obj.uniformData("lightPos", camera.getPosition());
    obj.uniformData("eyePos", camera.getPosition());
    static int t = 5;
    if(Keyboard::isKeyPressed(SDLK_n)) t++;
    if(Keyboard::isKeyPressed(SDLK_b)) t--;
    if(Keyboard::isKeyHeld(SDLK_h)) std::cout << "t = " << t << std::endl;
    for(int z = -t;z<=t;z++){
        for(int y = -t;y<=t;y++){
            for(int x = -t;x<=t;x++){
                glm::vec3 pos = glm::vec3(x, y, z);
                if(frustum.collide(pos, 1)){
                    obj.setPosition(pos);
                    obj.show(VP, true, true);
                }
            }
        }
    }
    
    frameBuffer.remove();
    
    obj2.use();
    frameBuffer.getDepthBuffer().use(0);
    obj2.uniformData("texture0", 0);
    frameBuffer.getColorBuffer().use(1);
    obj2.uniformData("texture1", 1);
    obj2.uniformData("option", option);
    obj2.show();
    frameBuffer.getColorBuffer().remove();
    frameBuffer.getDepthBuffer().remove();
    obj2.remove();
}

void Fenetre::run(){
    Mouse::capturePointer(true);
    int max = 10, i = 0;
    long tab[max];
    while(!isClosed()){
        auto time = SDL_GetTicks();
        updateInput();
        if(Keyboard::isKeyPressed(SDLK_LSHIFT))Mouse::capturePointer(false);
        if(Keyboard::isKeyReleased(SDLK_LSHIFT))Mouse::capturePointer(true);
        
        if(Keyboard::isKeyPressed(SDLK_ESCAPE))
            close();
        
        if(Keyboard::isKeyPressed(SDLK_r)) option = 1;
        if(Keyboard::isKeyPressed(SDLK_t)) option = 2;
        if(Keyboard::isKeyPressed(SDLK_y)) option = 3;
        
        camera.move();
        
        paintComponent();
        present();
        if(i == 0){
            tab[i] = SDL_GetTicks()-time;
            std::cout << max*1000.0f/tab[max-1] << " FPS" << std::endl;
        }else
            tab[i] = tab[i-1] + SDL_GetTicks()-time;
        i = (i+1)%max;
    }
}
