//
//  Frame.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 28/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Frame.hpp"


Frame::Frame() : target(0), position(0), pos(0, 0, 0), scale(1){
    
    lookTowardTarget(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
}
Frame::Frame(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis, glm::vec3 speed, glm::vec3 rotationSpeed, float scale) : target(0), position(0), speed(speed), rotationSpeed(rotationSpeed), pos(position), scale(scale){
    
    lookTowardTarget(target, verticalAxis);
}
Frame::~Frame(){
    
}

void Frame::update(){
    if(position != 0)
        pos = (*position)();
    
    if(target != 0)
        lookTowardTarget((*target)(), Y);
}
void Frame::move(FrameMove direction, float time){
    if(direction == FORWARD) pos += Z*time*speed.z;
    if(direction == BACKWARD) pos -= Z*time*speed.z;
    if(direction == LEFT) pos += X*time*speed.x;
    if(direction == RIGHT) pos -= X*time*speed.x;
    if(direction == UP) pos += Y*time*speed.y;
    if(direction == DOWN) pos -= Y*time*speed.y;
}
void Frame::rotate(FrameMove rotation, float time){
    if(rotation == YAW) rotate(Y, time*rotationSpeed.y);
    if(rotation == PITCH) rotate(X, time*rotationSpeed.x);
    if(rotation == ROLL) rotate(Z, time*rotationSpeed.z);
}

void Frame::rotate(float yaw, float pitch, float roll){
    if(yaw != 0) rotate(Y, yaw);
    if(pitch != 0) rotate(X, pitch);
    if(roll != 0) rotate(Z, roll);
}
void Frame::rotate(glm::vec3 axis, float angle){
    auto rot = glm::rotate(angle, axis);
    if(X != axis) X = glm::vec3(rot*glm::vec4(X, 0));
    if(Y != axis) Y = glm::vec3(rot*glm::vec4(Y, 0));
    if(Z != axis) Z = glm::vec3(rot*glm::vec4(Z, 0));
}

void Frame::lookAt(glm::mat4 &mat){
    mat = lookAt();
}
glm::mat4 Frame::lookAt(){
    if(scale != 1)
        return glm::lookAt(pos, pos+Z, Y)*glm::scale(glm::vec3(scale, scale, scale));
    else
        return glm::lookAt(pos, pos+Z, Y);
}
void Frame::toMatrix(glm::mat4 &mat){
    mat = toMatrix();
}
glm::mat4 Frame::toMatrix(){
    if(scale != 1)
        return glm::mat4(scale*glm::vec4(X, 0), scale*glm::vec4(Y, 0), scale*glm::vec4(Z, 0), glm::vec4(pos, 1));
    else
        return glm::mat4(glm::vec4(X, 0), glm::vec4(Y, 0), glm::vec4(Z, 0), glm::vec4(pos, 1));
}

void Frame::lookTowardTarget(glm::vec3 target, glm::vec3 verticalAxis){
    Y = verticalAxis;
    Y = glm::normalize(Y);
    Z = target-pos;
    Z = glm::normalize(Z);
    X = glm::cross(Y, Z);
    X = glm::normalize(X);
    Y = glm::cross(Z, X);
}
void Frame::attachTarget(glm::vec3 (*target)()){
    this->target = target;
}
void Frame::dettachTarget(){
    target = 0;
}
bool Frame::targetAttached(){
    return target == 0;
}

void Frame::attachPosition(glm::vec3 (*position)()){
    this->position = position;
}
void Frame::dettachPosition(){
    position = 0;
}
bool Frame::positionAttached(){
    return position == 0;
}

void Frame::setPosition(glm::vec3 position){
    pos = position;
}
glm::vec3 Frame::getPosition(){
    return pos;
}
void Frame::setSpeed(glm::vec3 speed){
    this->speed = speed;
}
glm::vec3 Frame::getSpeed(){
    return speed;
}
void Frame::setRotationSpeed(glm::vec3 rotationSpeed){
    this->rotationSpeed = rotationSpeed;
}
glm::vec3 Frame::getRotationSpeed(){
    return rotationSpeed;
}

void Frame::setScale(float scale){
    this->scale = scale;
}
float Frame::getScale(){
    return scale;
}

glm::vec3 Frame::getXAxis(){
    return X;
}
glm::vec3 Frame::getYAxis(){
    return Y;
}
glm::vec3 Frame::getZAxis(){
    return Z;
}