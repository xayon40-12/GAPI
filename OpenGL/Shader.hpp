//
//  Shader.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 01/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

//maybe useful
//#define GLSL(src) #src

#ifdef WIN32
#include <GL/glew.h>

#elif __APPLE__
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#include <OpenGL/gl3.h>

#else
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#include <GL3/gl3.h>

#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct DataContainer{
    std::string name;
    std::vector<float> dataArray;
    int dataLength;
    unsigned long size;
};

class Shader
{
private:
    static int count;
    static std::map<std::string, GLuint> vertexIDs;
    static std::map<std::string, GLuint> geometryIDs;
    static std::map<std::string, GLuint> fragmentIDs;
    
protected:
    GLuint vertexID;
    GLuint geometryID;
    GLuint fragmentID;
    GLuint programID;
    
    std::string sourceVertex;
    std::string sourceGeometry;
    std::string sourceFragment;
    
    std::map<std::string, int> offset;
    GLuint vboID;
    GLuint iboID;
    GLuint vaoID;
    
    std::vector<DataContainer> glslData;
    unsigned int nbElements;
    std::vector<GLuint> indicies;
    bool useIndicies;
    
    bool linkProgram();
    
    bool compileShader(GLuint &shader, GLenum type, std::string const &sourceFile);
    void updateVBO(std::string glslName, std::vector<float> const &data);
    
public:
    Shader(std::string sourceVertex, std::string sourceFragment);
    Shader(std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment);
    //WARNING : copy constructor and copy operator only copy the OpenGL shader part, it doesn't copy data and indicies -> you need load new data and indicies and call reload()
    Shader(Shader const &toCopy);
    Shader& operator=(Shader const &toCopy);
    ~Shader();
    
    bool create();// compile the shader and link the buffers (verticies, colours...).   WARNING : it clear data and indices form RAM
    virtual bool reload();// if buffers changed, use reload() and not create().   WARNING : it clear data and indices form RAM
    void use();
    void remove();// don't destroy the shader, just stop using it until call of use()
    void draw(GLenum mode, GLint first, GLsizei count);// draw "count" vertices from "first" of type "mode"
    void draw(GLenum mode);// draw all verticies
    void drawElements(GLenum mode, std::vector<GLuint> indicies);// draw all verticies using index
    
    void loadData(std::string glslName, std::vector<float> const &data, int dataLength);// if used with the same name it replace the old array and dataLength
    bool updateData(std::string glslName, std::vector<float> const &data);//update the buffer in the graphic card
    void loadIndecies(std::vector<GLuint> indicies);// if used again it replace the indicies
    
    void clearBuffers();// clean each buffer (vertices, colours...)
    
    void uniformData(std::string name, int data);
    void uniformData(std::string name, float data);
    void uniformData(std::string name, double data);
    void uniformData(std::string name, std::vector<int> data, int elementLength = 0);//can be used for one vec in place of array of vec
    void uniformData(std::string name, std::vector<float> data, int elementLength = 0);//can be used for one vec in place of array of vec
    void uniformData(std::string name, std::vector<double> data, int elementLength = 0);//can be used for one vec in place of array of vec
    void uniformData(std::string name, glm::vec2 data);
    void uniformData(std::string name, glm::vec3 data);
    void uniformData(std::string name, glm::vec4 data);
    void uniformData(std::string name, std::vector<glm::vec2> data);
    void uniformData(std::string name, std::vector<glm::vec3> data);
    void uniformData(std::string name, std::vector<glm::vec4> data);
    
    void uniformMatrix(std::string name, glm::mat4 mat);
    void uniformMatrix(std::string name, glm::mat3 mat);
    void uniformMatrix(std::string name, glm::mat2 mat);
    void uniformMVP(glm::mat4 MVP);
    
    GLint uniformLocation(std::string name);
    
    GLuint getProgram();
    GLuint getVAO();
    GLuint getVBO();
};

#endif /* Shader_hpp */
