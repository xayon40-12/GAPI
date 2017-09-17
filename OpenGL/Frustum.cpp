//
//  Frustum.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 19/06/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Frustum.hpp"

#include <iostream>

Frustum::Frustum(glm::mat4 matrix){
    glm::vec4 v;
    glm::vec3 n;
    float l;
    matrix = glm::transpose(matrix);// operator[] return line vector, so transpose to have column vector
    for(int i = 0;i<6;i++){
        v = matrix[3] + (i<3?1.0f:-1.0f)*matrix[i%3];
        n = glm::vec3(v);
        l = glm::length(n);
        planes[i] = {n/l, v.w/l};//
    }
}
Frustum::~Frustum() = default;

bool Frustum::collide(const glm::vec3 &pos, float radius){
    for (auto &plane : planes) {
        if(glm::dot(plane.normal, pos)+ plane.w+radius <= 0)
            return false;
    }
    return true;
}