//
//  Object.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 28/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "Shader.hpp"
#include "Frame.hpp"
#include "Texture.hpp"

/*
 GLSL data names:
 in_Vertex
 in_Colour
 in_TexCoord
 in_Normal
 in_Tangent
 in_BiTangent
 
 Texture names:
 texture0
 normalMap
 highMap
 
 Matrix:
 MVP
 model
 
 ┌───────────────┐
 │    WARNING    │
 │      !!       │
 │ PARTICULARITY │
 └───────────────┘
        For graphics memory optimisation an identical shape (vertices, normals...) is only stored once in graphics memory:
            -the shape is regular centered to (0, 0, 0) in the graphics memory, it is the model matrix which gives to the shape the properties of the object (scale, position, rotation)
            -an identical shape correspond to a shape with same: texture repetition, colour, tesselation, type (square, cube...)
 */

struct Object_t{
    std::vector<float> vertices, colours, normals, tangents, bitangents;
    std::vector<float> texCoord;
    //the atribute with Length correspond to the number of elements to make on data in each corresponding buffer. For exemple if vertexLength = 3 it means that one vertex is made with 3 floats from vertices buffer
    unsigned int vertexLength, colourLength, normalLength, tangentLength, bitangentLength, texCoordLength;
    std::vector<GLuint> indicies;
};

struct Shape{
    GLuint vao, vbo, ibo;
    std::vector<DataContainer> glslData;
    unsigned int nbElements;
    bool useIndices;
    unsigned int count;
};

class Object : public Frame, public Shader{
private:
    static std::map<std::string, Shape> shapes;
    
    using Shader::updateData;
protected:
    Texture *texture, *normalMap, *highMap;
    
    std::string type;
    glm::mat4 anchor;
    glm::mat3 baseAnchor;
    
    Object(std::string sourceVertex, std::string sourceFragment);
    Object(std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment);
public:
    //WARNING: If you copy an object you need to call reload() for the copied object
    Object(Object const &toCopy);
    Object& operator=(Object const &toCopy);
    ~Object();
    
    bool reload();
    
    void show(glm::mat4 viewProjection = glm::mat4(1), bool uniformModel = false, bool uniformBase = false);
    
    //************************************************* GENERATE/CREATE *************************************************
    //GENERATE means: generate the buffers (vertices, texCoord, normals...) and store it in an Object_t type
    //CREATE means: generate the buffers and use them to create an Object type with them
    //
    //the initialPostion and the X, Y, Z axis represent the initial position, orientation ans scale.
    //So if the initialPosition is (1, 0, 0) and after you use rotate(YAW, PI/2) the position will be (0, 0, -1)
    
    static Object_t generateSquare(glm::vec3 position, glm::vec3 X, glm::vec3 Y, glm::vec3 colour = {1, 1, 1}, glm::vec2 textureRepetition = {1, 1});
    static Object createSquare(glm::vec3 initialPosition, glm::vec3 X, glm::vec3 Y, std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment, glm::vec3 colour = {1, 1, 1}, std::string texture = "", glm::vec2 textureRepetition = {1, 1}, std::string normalMap = "", std::string highMap = "");
    
    /*For cube use one texture whith all faces on it
     ┌───┬───┬───┐
     │ F │ L │ U │  F:front   L:left   U:up
     ├───┼───┼───┤
     │ B │ R │ D │  B:back    R:right  D:down
     └───┴───┴───┘
     The texture repetition is per face
     */
    static Object_t generateCube(glm::vec3 position, glm::vec3 X, glm::vec3 Y, glm::vec3 Z, glm::vec3 colour = {1, 1, 1});
    static Object createCube(glm::vec3 initialPosition, glm::vec3 X, glm::vec3 Y, glm::vec3 Z, std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment, glm::vec3 colour = {1, 1, 1}, std::string texture = "", std::string normalMap = "", std::string highMap = "");
    
    static Object_t generateSphere(glm::vec3 position, float radius, unsigned int tesselation, glm::vec3 colour = {1, 1, 1});
    static Object createSphere(glm::vec3 initialPosition, glm::vec3 X, glm::vec3 Y, glm::vec3 Z, unsigned int tesselation, std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment, glm::vec3 colour = {1, 1, 1}, std::string texture = "", std::string normalMap = "", std::string highMap = "");
};

#endif /* Object_hpp */
