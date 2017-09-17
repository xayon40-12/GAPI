//
//  Frame.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 28/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Frame.hpp"


Frame::Frame() : attachedTarget(nullptr), attachedPosition(nullptr), pos(0, 0, 0), scale(1){
    
    lookTowardTarget(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
}
Frame::Frame(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &verticalAxis, const glm::vec3 &speed,
             const glm::vec3 &rotationSpeed, float scale) : attachedTarget(
        nullptr), attachedPosition(nullptr), speed(speed), rotationSpeed(rotationSpeed), pos(position), scale(scale){
    
    lookTowardTarget(target, verticalAxis);
}
Frame::~Frame() = default;

void Frame::update(){
    if(attachedPosition != nullptr)
        pos = (*attachedPosition)();
    
    if(attachedTarget != nullptr)
        lookTowardTarget((*attachedTarget)(), Y);
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
void Frame::rotate(const glm::vec3 &axis, float angle){
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

void Frame::lookTowardTarget(const glm::vec3 &target, const glm::vec3 &verticalAxis){
    Y = verticalAxis;
    Y = glm::normalize(Y);
    Z = target-pos;
    Z = glm::normalize(Z);
    X = glm::cross(Y, Z);
    X = glm::normalize(X);
    Y = glm::cross(Z, X);
}
void Frame::attachTarget(glm::vec3 (*target)()){
    this->attachedTarget = target;
}
void Frame::dettachTarget(){
    attachedTarget = nullptr;
}
bool Frame::targetAttached(){
    return attachedTarget == nullptr;
}

void Frame::attachPosition(glm::vec3 (*position)()){
    this->attachedPosition = position;
}
void Frame::dettachPosition(){
    attachedPosition = nullptr;
}
bool Frame::positionAttached(){
    return attachedPosition == nullptr;
}

void Frame::setPosition(const glm::vec3 &position){
    pos = position;
}
glm::vec3 Frame::getPosition(){
    return pos;
}
void Frame::setSpeed(const glm::vec3 &speed){
    this->speed = speed;
}
glm::vec3 Frame::getSpeed(){
    return speed;
}
void Frame::setRotationSpeed(const glm::vec3 &rotationSpeed){
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