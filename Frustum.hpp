//
//  Frustum.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 19/06/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Frustum_hpp
#define Frustum_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Plane{
    glm::vec3 normal;
    float w;
};

class Frustum{
private:
    Plane planes[6];
public:
    /*
     The matrix can be the viewProjection matrix (projection*view)
     or the inverse of a model matrix which will generate a cube frustum around the pos in the model matrix
     */
    Frustum(glm::mat4 matrix);
    ~Frustum();
    
    bool collide(glm::vec3 pos, float radius);
};

#endif /* Frustum_hpp */
