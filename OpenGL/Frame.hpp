//
//  Frame.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 28/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Frame_hpp
#define Frame_hpp

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum FrameMove {UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD, YAW, PITCH, ROLL};

class Frame{
protected:
    glm::vec3 (*attachedTarget)();
    glm::vec3 (*attachedPosition)();
    
    glm::vec3 pos;
    glm::vec3 speed;
    glm::vec3 rotationSpeed;
    
    glm::vec3 X, Y, Z;
    
    float scale;
    
public:
    Frame();
    Frame(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &verticalAxis, const glm::vec3 &speed,
          const glm::vec3 &rotationSpeed, float scale = 1);
    virtual ~Frame();
    
    void update();//change position and look toward target if they are attached
    void move(FrameMove direction, float time = 1);//uses speed which is multiplyed by "time"
    
    void rotate(float yaw, float pitch, float roll);
    virtual void rotate(const glm::vec3 &axis, float angle);
    void rotate(FrameMove rotation, float time);//uses the rotationSpeed which is multiplyed by "time". Rotate trigonomicaly, use negative "time" to rotate clockwise.
    
    void lookAt(glm::mat4 &mat);
    glm::mat4 lookAt();//like OpenGL lookAt
    void toMatrix(glm::mat4 &mat);
    virtual glm::mat4 toMatrix();//return the matrix of the coordinate system
    
    virtual void lookTowardTarget(const glm::vec3 &target, const glm::vec3 &verticalAxis);
    void attachTarget(glm::vec3 (*target)());//automaticaly look toward the attachedTarget when update()
    void dettachTarget();
    bool targetAttached();
    
    void attachPosition(glm::vec3 (*position)());//automaticaly positioning to "attachedPosition" when update()
    void dettachPosition();
    bool positionAttached();
    
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition();
    void setSpeed(const glm::vec3 &speed);
    glm::vec3 getSpeed();
    void setRotationSpeed(const glm::vec3 &rotationSpeed);
    glm::vec3 getRotationSpeed();
    
    void setScale(float scale);
    float getScale();
    
    glm::vec3 getXAxis();
    glm::vec3 getYAxis();
    glm::vec3 getZAxis();
};

#endif /* Frame_hpp */
