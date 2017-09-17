//
//  Camera.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 23/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera() : Frame(){
    initMapping();
    
    lookTowardTarget(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}
Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &verticalAxis,
               const glm::vec3 &speed, const glm::vec3 &rotationSpeed, bool verticalFree) : Frame(position, target, verticalAxis, speed, rotationSpeed), verticalFree(verticalFree){
    initMapping();
    
    lookTowardTarget(target, verticalAxis);
}
Camera::~Camera() = default;

void Camera::initMapping(){
    mapping[UP] = SDLK_SPACE;
    mapping[DOWN] = SDLK_LSHIFT;
    mapping[LEFT] = SDLK_q;
    mapping[RIGHT] = SDLK_d;
    mapping[FORWARD] = SDLK_z;
    mapping[BACKWARD] = SDLK_s;
    mapping[YAW] = MOUSEX;
    mapping[PITCH] = MOUSEY;
    mapping[ROLL] = KEYBOARD;
    
    mappingRotation[YAW] = {-1, -1};
    mappingRotation[PITCH] = {-1, -1};
    if(verticalFree)
        mappingRotation[ROLL] = {SDLK_a, SDLK_e};
    else
        mappingRotation[ROLL] = {-1, -1};
}

void Camera::move(){
    if(attachedPosition != nullptr){
        pos = (*attachedPosition)();
    }else{
        if(Keyboard::isKeyHeld(mapping[FORWARD])) pos += Z*speed.z;
        if(Keyboard::isKeyHeld(mapping[BACKWARD])) pos -= Z*speed.z;
        if(Keyboard::isKeyHeld(mapping[LEFT])) pos += X*speed.x;
        if(Keyboard::isKeyHeld(mapping[RIGHT])) pos -= X*speed.x;
        if(Keyboard::isKeyHeld(mapping[UP])) pos += Y*speed.y;
        if(Keyboard::isKeyHeld(mapping[DOWN])) pos -= Y*speed.y;
    }
    
    if(attachedTarget != nullptr){
        lookTowardTarget((*attachedTarget)(), Y);
    }else{
        inputRotate(YAW);
        inputRotate(PITCH);
        inputRotate(ROLL);
    }
}

void Camera::inputRotate(FrameMove mapping){
    switch(this->mapping[mapping]){
        case KEYBOARD:
            if(Keyboard::isKeyHeld(mappingRotation[mapping].first)) Frame::rotate(mapping, -1);
            if(Keyboard::isKeyHeld(mappingRotation[mapping].second)) Frame::rotate(mapping, 1);
            break;
        case MOUSEX:
            Frame::rotate(mapping, -Mouse::xRelative());
            break;
        case MOUSEY:
            Frame::rotate(mapping, Mouse::yRelative());
            break;
        case WHEELX:
            Frame::rotate(mapping, Mouse::xWheel());
            break;
        case WHEELY:
            Frame::rotate(mapping, Mouse::yWheel());
            break;
        default:
            break;
    }
}

void Camera::map(FrameMove mapping, SDL_Keycode key){
    this->mapping[mapping] = key;
}
void Camera::mapRotation(FrameMove mapping, SDL_Keycode key1, SDL_Keycode key2){
    this->mapping[mapping] = KEYBOARD;
    mappingRotation[mapping] = {key1, key2};
}

void Camera::rotate(const glm::vec3 &axis, float angle){
    if(axis == Z && !verticalFree) return;
    auto rot = glm::rotate(angle, axis);
    if(X != axis) X = glm::vec3(rot*glm::vec4(X, 0));
    if(Y != axis) if(axis != X || verticalFree) Y = glm::vec3(rot*glm::vec4(Y, 0));
    if(Z != axis) Z = glm::vec3(rot*glm::vec4(Z, 0));
    if(!verticalFree && glm::dot(Z, glm::cross(X, Y))<0){
        Z = glm::normalize((glm::dot(Z, Y)>0?1.0f:-1.0f)*Y+0.01f*glm::cross(X, Y));
    }
}
void Camera::lookTowardTarget(const glm::vec3 &target, const glm::vec3 &verticalAxis){
    Y = verticalAxis;
    Y = glm::normalize(Y);
    Z = target-pos;
    Z = glm::normalize(Z);
    X = glm::cross(Y, Z);
    X = glm::normalize(X);
    if(verticalFree)
        Y = glm::cross(Z, X);
}
glm::mat4 Camera::toMatrix(){
    glm::vec3 tmpY = Y;
    if(!verticalFree)
        tmpY = glm::cross(Z, X);
    return glm::mat4(scale*glm::vec4(X, 0), scale*glm::vec4(tmpY, 0), scale*glm::vec4(Z, 0), glm::vec4(pos, 1));
}

void Camera::setVerticalFree(bool enable){
    verticalFree = enable;
}
