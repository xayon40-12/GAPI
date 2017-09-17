//
//  Camera.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 23/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <map>

#include "../SDL/Keyboard.hpp"
#include "../SDL/Mouse.hpp"
#include "Frame.hpp"

enum CameraMouseMove {MOUSEX, MOUSEY, WHEELX, WHEELY, KEYBOARD};

class Camera : public Frame{
private:
    std::map<FrameMove, SDL_Keycode> mapping;
    std::map<FrameMove, std::pair<SDL_Keycode, SDL_Keycode>> mappingRotation;
    
    bool verticalFree;
    
    void initMapping();
    
    void inputRotate(FrameMove mapping);
public:
    Camera();
    Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &verticalAxis, const glm::vec3 &speed,
           const glm::vec3 &rotationSpeed, bool verticalFree = false);
    ~Camera() override;
    
    void move();//move the camera by using the keyboard mapping (read map() comments) and the speed
    
    void map(FrameMove mapping, SDL_Keycode key);//chose the key mapping for a specific move (forward, left...)
    void mapRotation(FrameMove mapping, SDL_Keycode key1, SDL_Keycode key2);//same as map() but for rotation. There is two keys: key1 for trigonometric (counterclockwise) rotation, key2 for clockwise rotation
    
    void rotate(const glm::vec3 &axis, float angle) override;
    void lookTowardTarget(const glm::vec3 &target, const glm::vec3 &verticalAxis) override;
    glm::mat4 toMatrix() override;
    
    void setVerticalFree(bool enable);//set if the vertical axis move or not when rotating
};

#endif /* Camera_hpp */
