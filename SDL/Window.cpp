//
//  Window.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 23/04/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Window.hpp"
#include "../OpenGL/GLWindow.hpp"

Window::Window(std::string const &title, int width, int height, bool visible, bool GL) : Event(), win(nullptr), winID(0), renderer(
        nullptr),
                                                                                  title(title), width(width),
                                                                                  height(height), visible(visible),
                                                                                  closed(false){
    if(!GL)
        create();
}

Window::~Window(){
    windows.erase(winID);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
}

void Window::create(){
    win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, visible?SDL_WINDOW_SHOWN:SDL_WINDOW_HIDDEN);
    winID = SDL_GetWindowID(win);
    windows[winID] = this;
    activeWindow = winID;
    operation = EXIT_ON_CLOSE;
    
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    update();
}

void Window::setTitle(const std::string &title){
    this->title = title;
    SDL_SetWindowTitle(win, title.c_str());
}
void Window::setSize(int width, int height){
    if(width > 0 && height > 0)
        SDL_SetWindowSize(win, width, height);
}
void Window::setDefaultCloseOperation(CloseOperation operation){
    this->operation = operation;
}

std::string Window::getTitle(){
    return title;
}
int Window::getWidth(){
    return width;
}
int Window::getHeight(){
    return height;
}
Uint32 Window::getID(){
    return winID;
}

void Window::close(){
    closed = true;
    setVisible(false);
}
bool Window::isClosed(){
    //return SDL_GetWindowFlags(win)&SDL_WINDOW_HIDDEN;
    return closed;
}
bool Window::isActive(){
    return winID==activeWindow;
}
bool Window::isVisible(){
    return visible;
}

void Window::updateInput(){
    Event::update();
}
void Window::update(){
    updateInput();
    Window::present();
}

void Window::setVisible(bool visible){
    this->visible = visible;
    if(visible)
        SDL_ShowWindow(win);
    else
        SDL_HideWindow(win);
    update();
}
void Window::setFullscreen(bool fullscreen){
    SDL_SetWindowFullscreen(win, fullscreen?SDL_WINDOW_FULLSCREEN:0);
}
void Window::setVsync(bool enable){
    SDL_GL_SetSwapInterval(enable);
}
void Window::setPosition(int x, int y){
    SDL_SetWindowPosition(win, x, y);
}

//************************ Graphics ************************

void Window::clear(){
    SDL_RenderClear(renderer);
}

void Window::setColour(const Colour &c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void Window::drawRect(int x, int y, int w, int h){
    SDL_Rect r = {x, y, w, h};
    SDL_RenderDrawRect(renderer, &r);
}
void Window::fillRect(int x, int y, int w, int h){
    SDL_Rect r = {x, y, w, h};
    SDL_RenderFillRect(renderer, &r);
}

void Window::drawLine(int x1, int y1, int x2, int y2){
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void Window::drawPoint(int x, int y){
    SDL_RenderDrawPoint(renderer, x, y);
}

void Window::present(){
    SDL_RenderPresent(renderer);
}
