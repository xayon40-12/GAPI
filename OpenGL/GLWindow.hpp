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

    void create() override;
public:
    GLWindow(std::string title, int width, int height, bool visible = true, int major = 2, int minor = 1);
    ~GLWindow() override;
    
    static void initOpenGL(int major = 2, int minor = 1);
    
    //************************ Graphics ************************

    void clear() override;

    void setColour(const Colour &c) override;

    void drawRect(int x, int y, int w, int h) override;

    void fillRect(int x, int y, int w, int h) override;

    void drawLine(int x1, int y1, int x2, int y2) override;

    void drawPoint(int x, int y) override;

    void present() override;
};

#endif /* GLWindows_hpp */
