//
//  GLWindows.cpp
//  SDLlib
//
//  Created by Nathan Touroux on 05/09/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "GLWindow.hpp"

bool GLWindow::s_glInit = false;

void GLWindow::initOpenGL(int major, int minor){
    if(major>2){
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }else{
        glOrtho(0, ((GLWindow*)windows[activeWindow])->getWidth(), ((GLWindow*)windows[activeWindow])->getHeight(), 0, -1, 1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    s_glInit = true;
}

GLWindow::GLWindow(std::string title, int width, int height, bool visible, int major, int minor) :Window(title, width, height, visible, true), major(major), minor(minor){
    create();
}

GLWindow::~GLWindow(){
    SDL_GL_DeleteContext(context);
}

void GLWindow::create(){
    win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, visible?SDL_WINDOW_SHOWN:SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
    winID = SDL_GetWindowID(win);
    windows[winID] = this;
    activeWindow = winID;
    operation = EXIT_ON_CLOSE;
    
    if(!s_glInit) initOpenGL(major, minor);
    context = SDL_GL_CreateContext(win);
}

//************************ Graphics ************************

void GLWindow::clear(){
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::setColour(Colour c){
    color = {c.r/255.0f, c.g/255.0f, c.b/255.0f, c.a/255.0f};
    glColor4f(color.r, color.g, color.b, color.a);
}

void GLWindow::drawRect(int x, int y, int w, int h){
    glBegin(GL_LINE_STRIP);
    glVertex2i(x, y);
    glVertex2i(x, y+h);
    glVertex2i(x+w, y+h);
    glVertex2i(x+w, y);
    glVertex2i(x, y);
    glEnd();
}
void GLWindow::fillRect(int x, int y, int w, int h){
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x, y+h);
    glVertex2i(x+w, y+h);
    glVertex2i(x+w, y);
    glEnd();
}

void GLWindow::drawLine(int x1, int y1, int x2, int y2){
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}
void GLWindow::drawPoint(int x, int y){
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void GLWindow::present(){
    SDL_GL_SwapWindow(win);
}
