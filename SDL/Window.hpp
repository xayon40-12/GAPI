//
//  Window.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 23/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <iostream>
#include "Event.hpp"
#include "Colour.hpp"

class Window : public Event{
protected:    
    SDL_Window *win;
    Uint32 winID;
    SDL_Renderer *renderer;
    
    std::string title;
    int width, height;
    bool visible;
    bool closed;

    CloseOperation operation;
    
    virtual void create();
    //Window(bool GL, std::string title, int width, int height, bool visible = true);
public:
    Window(std::string const &title, int width, int height, bool visible = true, bool GL = false);

    ~Window() override;
    
    void setTitle(const std::string &title);
    void setSize(int width, int height);
    void setDefaultCloseOperation(CloseOperation operation);
    
    std::string getTitle();
    int getWidth();
    int getHeight();
    Uint32 getID();
    CloseOperation getDefaultCloseOperation();
    
    void close();
    bool isClosed();
    bool isActive();
    bool isVisible();
    
    void updateInput();
    void update();
    
    void setVisible(bool visible);
    void setFullscreen(bool fullscreen);
    void setVsync(bool enable);
    void setPosition(int x, int y);

    
    //************************ Graphics ************************
    
    virtual void clear();
    
    virtual void setColour(const Colour &c);
    
    virtual void drawRect(int x, int y, int w, int h);
    virtual void fillRect(int x, int y, int w, int h);
    virtual void drawLine(int x1, int y1, int x2, int y2);
    virtual void drawPoint(int x, int y);
    
    virtual void present();
};

#endif /* Frame_hpp */
