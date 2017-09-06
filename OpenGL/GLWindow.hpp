//
//  GLWindows.hpp
//  SDLlib
//
//  Created by Nathan Touroux on 05/09/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef GLWindows_hpp
#define GLWindows_hpp

#include "../SDL/Window.hpp"
#include <SDL2/SDL_opengl.h>

class GLWindow : public Window {
private:
    static bool s_glInit;
    
    struct{float r, g, b, a;} color;
protected:
    SDL_GLContext context;
    int major, minor;
    
    virtual void create();
public:
    GLWindow(std::string title, int width, int height, bool visible = true, int major = 2, int minor = 1);
    ~GLWindow();
    
    static void initOpenGL(int major = 2, int minor = 1);
    
    //************************ Graphics ************************
    
    virtual void clear();
    
    virtual void setColour(Colour c);
    
    virtual void drawRect(int x, int y, int w, int h);
    virtual void fillRect(int x, int y, int w, int h);
    virtual void drawLine(int x1, int y1, int x2, int y2);
    virtual void drawPoint(int x, int y);
    
    virtual void present();
};

#endif /* GLWindows_hpp */
