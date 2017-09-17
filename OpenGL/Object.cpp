//
//  Object.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 28/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Object.hpp"

std::map<std::string, Shape> Object::shapes;

Object::Object(const std::string &sourceVertex, const std::string &sourceFragment): Frame(), Shader(sourceVertex, sourceFragment), texture(
        nullptr), normalMap(nullptr), highMap(nullptr){
}
Object::Object(const std::string &sourceVertex, const std::string &sourceGeometry, const std::string &sourceFragment): Frame(), Shader(sourceVertex, sourceGeometry, sourceFragment), texture(
        nullptr), normalMap(nullptr), highMap(nullptr){
}
Object::Object(Object const &toCopy): Frame(toCopy), Shader(toCopy), texture(nullptr), normalMap(nullptr), highMap(
        nullptr), type(toCopy.type), anchor(toCopy.anchor), baseAnchor(toCopy.baseAnchor){
    delete texture; texture = nullptr;
    delete normalMap; normalMap = nullptr;
    delete highMap; highMap = nullptr;
    if(toCopy.texture) texture = new Texture(*toCopy.texture);
    if(toCopy.normalMap) normalMap = new Texture(*toCopy.normalMap);
    if(toCopy.highMap) highMap = new Texture(*toCopy.highMap);
}
Object& Object::operator=(Object const &toCopy){
    Frame::operator=(toCopy);
    Shader::operator=(toCopy);
    delete texture; texture = nullptr;
    delete normalMap; normalMap = nullptr;
    delete highMap; highMap = nullptr;
    if(toCopy.texture) texture = new Texture(*toCopy.texture);
    if(toCopy.normalMap) normalMap = new Texture(*toCopy.normalMap);
    if(toCopy.highMap) highMap = new Texture(*toCopy.highMap);
    type = toCopy.type;
    anchor = toCopy.anchor;
    baseAnchor = toCopy.baseAnchor;
    return *this;
}
Object::~Object(){
    delete texture;
    delete normalMap;
    delete highMap;
    vaoID = 0;
    vboID = 0;
    iboID = 0;
    auto tmp = shapes.find(type);
    if(tmp != shapes.end()){
        Shape &s = tmp->second;
        s.count--;
        if(s.count == 0){
            if(glIsBuffer(s.vbo) == GL_TRUE)
                glDeleteBuffers(1, &s.vbo);
            if(glIsBuffer(s.ibo) == GL_TRUE)
                glDeleteBuffers(1, &s.ibo);
            if(glIsVertexArray(s.vao) == GL_TRUE)
                glDeleteVertexArrays(1, &s.vao);
            shapes.erase(tmp);
        }
    }
}

bool Object::reload(){
    if(vaoID != 0)return true;
    
    auto tmp = shapes.find(type);
    if(tmp != shapes.end()){
        Shape &s = tmp->second;
        vaoID = s.vao;
        vboID = s.vbo;
        iboID = s.ibo;
        glslData = s.glslData;
        nbElements = s.nbElements;
        useIndicies = s.useIndices;
        if(linkProgram()){
            s.count++;
            return true;
        }else{
            vaoID = 0;
            vboID = 0;
            iboID = 0;
            return false;
        }
    }else{
        if(Shader::reload()){
            clearBuffers();
            shapes[type] = {vaoID, vboID, iboID, glslData, nbElements, useIndicies, 1};
            return true;
        }else
            return false;
    }
}

void Object::show(const glm::mat4 &viewProjection, bool uniformModel, bool uniformBase){
    glm::mat4 model = toMatrix()*anchor, MVP = viewProjection*model;
    //use frustum culling
    use();
    if(texture){
        texture->use(0);
        uniformData("texture0", 0);
    }
    if(normalMap){
        normalMap->use(1);
        uniformData("normalMap", 1);
    }
    if(highMap){
        highMap->use(2);
        uniformData("highMap", 2);
    }
    uniformMVP(MVP);
    if(uniformModel) uniformMatrix("model", model);
    if(uniformBase) uniformMatrix("base", glm::mat3(glm::vec3(X), glm::vec3(Y), glm::vec3(Z))*baseAnchor);
    draw(GL_TRIANGLES);
    if(texture) texture->remove();
    if(normalMap) normalMap->remove();
    if(highMap) highMap->remove();
    remove();
}

//************************************************* GENERATE *************************************************
/*
 
 types:
    -a type of shape is stored in a map with name (std::string) which is written with this typology:
        -colour: (r,g,b)
        -repetition: rx:ry
        -tesselation: t
    -shapes:
        -square: "square(r,g,b)rx:ry"
 */
#include <sstream>
Object_t Object::generateSquare(const glm::vec3 &position, const glm::vec3 &X, const glm::vec3 &Y,
                                const glm::vec3 &colour, const glm::vec2 &textureRepetition){
    Object_t obj;
    
    glm::vec3 v1(position-X+Y), v2(position-X-Y), v3(position+X-Y), v4(position+X+Y);
    std::vector<float> vertices = {v1.x, v1.y, v1.z,    v2.x, v2.y, v2.z,    v3.x, v3.y, v3.z,    v4.x, v4.y, v4.z}, colours, normals, tangents, bitangents;
    std::vector<float> coord = {0, textureRepetition.y,   0, 0,   textureRepetition.x, 0,   textureRepetition.x, textureRepetition.y};
    std::vector<GLuint> indicies = {0, 1, 2,    0, 2, 3};
    
    glm::vec3 T(X), B(Y), N;
    N = glm::cross(T, B);
    T = glm::normalize(T);
    B = glm::normalize(B);
    N = glm::normalize(N);

    for(int i = 0;i<4;i++){
        colours.push_back(colour.x);
        colours.push_back(colour.y);
        colours.push_back(colour.z);
        
        normals.push_back(N.x);
        normals.push_back(N.y);
        normals.push_back(N.z);
        
        tangents.push_back(T.x);
        tangents.push_back(T.y);
        tangents.push_back(T.z);
        
        bitangents.push_back(B.x);
        bitangents.push_back(B.y);
        bitangents.push_back(B.z);
    }
    
    obj.vertices = vertices; obj.vertexLength = 3;
    obj.colours = colours; obj.colourLength = 3;
    obj.texCoord = coord; obj.texCoordLength = 2;
    obj.normals = normals; obj.normalLength = 3;
    obj.tangents = tangents; obj.tangentLength = 3;
    obj.bitangents = bitangents; obj.bitangentLength = 3;
    obj.indicies = indicies;
    
    return obj;
}
Object Object::createSquare(const glm::vec3 &initialPosition, const glm::vec3 &X, const glm::vec3 &Y,
                            const std::string &sourceVertex, const std::string &sourceGeometry,
                            const std::string &sourceFragment, const glm::vec3 &colour, const std::string &texture,
                            const glm::vec2 &textureRepetition, const std::string &normalMap,
                            const std::string &highMap){
    Object obj(sourceVertex, sourceGeometry, sourceFragment);
    
    std::ostringstream oss;
    oss << "square" << "(" << colour.r << "," << colour.g << "," << colour.b << ")" << textureRepetition.x << "x" << textureRepetition.y;
    obj.type = oss.str();
    
    glm::vec3 x(X), y(Y), z;
    float lx = glm::length(x), ly = glm::length(y);
    x /= lx;
    y /= ly;
    z = glm::cross(x, y);
    z = glm::normalize(z);
    obj.anchor = glm::mat4(glm::vec4(X, 0), glm::vec4(Y, 0), glm::vec4(z, 0), glm::vec4(initialPosition, 1));
    obj.baseAnchor = glm::mat3(x, y, z);
    
    if(!texture.empty()){
        obj.texture = new Texture(texture);
        obj.texture->load();
    }
    if(!normalMap.empty()){
        obj.normalMap = new Texture(normalMap);
        obj.normalMap->load();
    }
    if(!highMap.empty()){
        obj.highMap = new Texture(highMap);
        obj.highMap->load();
    }
    
    if(shapes.find(obj.type) == shapes.end()){
        Object_t tmp = generateSquare(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), colour, textureRepetition);
        
        obj.loadData("in_Vertex", tmp.vertices, tmp.vertexLength);
        obj.loadData("in_Colour", tmp.colours, tmp.colourLength);
        obj.loadData("in_TexCoord", tmp.texCoord, tmp.texCoordLength);
        obj.loadData("in_Normal", tmp.normals, tmp.normalLength);
        obj.loadData("in_Tangent", tmp.tangents, tmp.tangentLength);
        obj.loadData("in_BiTangent", tmp.bitangents, tmp.bitangentLength);
        obj.loadIndecies(tmp.indicies);
    }
    
    return obj;
}

Object_t Object::generateCube(const glm::vec3 &position, const glm::vec3 &X, const glm::vec3 &Y, const glm::vec3 &Z,
                              const glm::vec3 &colour){
    Object_t obj;
    
    std::vector<float> vertices, colours, normals, tangents, bitangents;
    std::vector<float> coord;
    std::vector<GLuint> indicies;
    for(GLuint i = 0;i<6;i++){
        indicies.push_back(0 + 4*i);
        indicies.push_back(1 + 4*i);
        indicies.push_back(2 + 4*i);
        indicies.push_back(0 + 4*i);
        indicies.push_back(2 + 4*i);
        indicies.push_back(3 + 4*i);
    }
    
    auto fill = [&](glm::vec3 T, glm::vec3 B, glm::vec3 N, int tx, int ty){
        glm::mat3 TBN = glm::mat3(X, Y, Z)*glm::mat3(T, B, N);
        T = TBN[0]; B = TBN[1]; N = TBN[2];
        glm::vec3 v1(position+N-T+B), v2(position+N-T-B), v3(position+N+T-B), v4(position+N+T+B);
        std::vector<float> vert = {v1.x, v1.y, v1.z,    v2.x, v2.y, v2.z,    v3.x, v3.y, v3.z,    v4.x, v4.y, v4.z};
        vertices.insert(vertices.end(), vert.begin(), vert.end());
        
        std::vector<float> co = {tx/3.0f, (ty+1)/2.0f,    tx/3.0f, ty/2.0f,    (tx+1)/3.0f, ty/2.0f,    (tx+1)/3.0f, (ty+1)/2.0f};
        coord.insert(coord.end(), co.begin(), co.end());
        for(int i = 0;i<4;i++){
            colours.push_back(colour.x);
            colours.push_back(colour.y);
            colours.push_back(colour.z);
            
            normals.push_back(N.x);
            normals.push_back(N.y);
            normals.push_back(N.z);
            
            tangents.push_back(T.x);
            tangents.push_back(T.y);
            tangents.push_back(T.z);
            
            bitangents.push_back(B.x);
            bitangents.push_back(B.y);
            bitangents.push_back(B.z);
        }
    };
    
    fill(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), 0, 1);
    fill(glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), 0, 0);
    fill(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), 1, 1);
    fill(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 1, 0);
    fill(glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 2, 1);
    fill(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0), 2, 0);
    
    obj.vertices = vertices; obj.vertexLength = 3;
    obj.colours = colours; obj.colourLength = 3;
    obj.texCoord = coord; obj.texCoordLength = 2;
    obj.normals = normals; obj.normalLength = 3;
    obj.tangents = tangents; obj.tangentLength = 3;
    obj.bitangents = bitangents; obj.bitangentLength = 3;
    obj.indicies = indicies;
    
    return obj;
}
Object Object::createCube(const glm::vec3 &initialPosition, const glm::vec3 &X, const glm::vec3 &Y, const glm::vec3 &Z,
                          const std::string &sourceVertex, const std::string &sourceGeometry,
                          const std::string &sourceFragment, const glm::vec3 &colour, const std::string &texture,
                          const std::string &normalMap, const std::string &highMap){
    Object obj(sourceVertex, sourceGeometry, sourceFragment);
    
    std::ostringstream oss;
    oss << "cube" << "(" << colour.r << "," << colour.g << "," << colour.b << ")";
    obj.type = oss.str();
    
    glm::vec3 x(X), y(Y), z(Z);
    x = glm::normalize(x);
    y = glm::normalize(y);
    z = glm::normalize(z);
    obj.anchor = glm::mat4(glm::vec4(X, 0), glm::vec4(Y, 0), glm::vec4(Z, 0), glm::vec4(initialPosition, 1));
    obj.baseAnchor = glm::mat3(x, y, z);
    
    if(!texture.empty()){
        obj.texture = new Texture(texture);
        obj.texture->load();
    }
    if(!normalMap.empty()){
        obj.normalMap = new Texture(normalMap);
        obj.normalMap->load();
    }
    if(!highMap.empty()){
        obj.highMap = new Texture(highMap);
        obj.highMap->load();
    }
    
    if(shapes.find(obj.type) == shapes.end()){
        Object_t tmp = generateCube(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), colour);
        
        obj.loadData("in_Vertex", tmp.vertices, tmp.vertexLength);
        obj.loadData("in_Colour", tmp.colours, tmp.colourLength);
        obj.loadData("in_TexCoord", tmp.texCoord, tmp.texCoordLength);
        obj.loadData("in_Normal", tmp.normals, tmp.normalLength);
        obj.loadData("in_Tangent", tmp.tangents, tmp.tangentLength);
        obj.loadData("in_BiTangent", tmp.bitangents, tmp.bitangentLength);
        obj.loadIndecies(tmp.indicies);
    }
    
    return obj;
    
}


Object_t Object::generateSphere(const glm::vec3 &position, float radius, unsigned int tesselation,
                                const glm::vec3 &colour){
    if(tesselation<2) tesselation = 2;
    
    Object_t obj;
    std::vector<float> vertices, colours, normals, tangents, bitangents;
    std::vector<float> coord;
    std::vector<GLuint> indicies;
    
    auto push = [&](glm::vec3 T, glm::vec3 B, glm::vec3 N, float tx, float ty){
        vertices.push_back(position.x+N.x*radius);
        vertices.push_back(position.y+N.y*radius);
        vertices.push_back(position.z+N.z*radius);
        
        colours.push_back(colour.x);
        colours.push_back(colour.y);
        colours.push_back(colour.z);
        
        coord.push_back(tx);
        coord.push_back(ty);
        
        normals.push_back(N.x);
        normals.push_back(N.y);
        normals.push_back(N.z);
        
        tangents.push_back(T.x);
        tangents.push_back(T.y);
        tangents.push_back(T.z);
        
        bitangents.push_back(B.x);
        bitangents.push_back(B.y);
        bitangents.push_back(B.z);
    };
    
    float PI = (float) M_PI, PI2 = (float) (M_PI * 2), step = PI / tesselation;
    for(int i = 0;i<=2*tesselation;i++){
        for(int j = 0;j<=tesselation;j++){
            float theta = i*step, phi = j*step;
            float ct = cosf(theta), st = sinf(theta), cp = cosf(phi), sp = sinf(phi);
            glm::vec3 N(-st*sp, cp, -ct*sp),
            T(-ct, 0, st),
            B(-st*cp, -sp, -ct*cp);
            push(T, B, N, theta/PI2, 1-phi/PI);
            if(j!=0  && j!=tesselation && i!=2*tesselation){
                indicies.push_back(i*(tesselation+1)+j-1);
                indicies.push_back(i*(tesselation+1)+j);
                indicies.push_back((i+1)*(tesselation+1)+j);
                
                indicies.push_back(i*(tesselation+1)+j);
                indicies.push_back((i+1)*(tesselation+1)+j);
                indicies.push_back((i+1)*(tesselation+1)+j+1);
            }
        }
    }
    
    obj.vertices = vertices; obj.vertexLength = 3;
    obj.colours = colours; obj.colourLength = 3;
    obj.texCoord = coord; obj.texCoordLength = 2;
    obj.normals = normals; obj.normalLength = 3;
    obj.tangents = tangents; obj.tangentLength = 3;
    obj.bitangents = bitangents; obj.bitangentLength = 3;
    obj.indicies = indicies;
    
    return obj;
}
Object Object::createSphere(const glm::vec3 &initialPosition, const glm::vec3 &X, const glm::vec3 &Y,
                            const glm::vec3 &Z, unsigned int tesselation, const std::string &sourceVertex,
                            const std::string &sourceGeometry, const std::string &sourceFragment,
                            const glm::vec3 &colour, const std::string &texture, const std::string &normalMap,
                            const std::string &highMap){
    Object obj(sourceVertex, sourceGeometry, sourceFragment);
    
    std::ostringstream oss;
    oss << "sphere" << "(" << colour.r << "," << colour.g << "," << colour.b << ")" << tesselation;
    obj.type = oss.str();
    
    glm::vec3 x(X), y(Y), z(Z);
    x = glm::normalize(x);
    y = glm::normalize(y);
    z = glm::normalize(z);
    obj.anchor = glm::mat4(glm::vec4(X, 0), glm::vec4(Y, 0), glm::vec4(Z, 0), glm::vec4(initialPosition, 1));
    obj.baseAnchor = glm::mat3(x, y, z);
    
    if(!texture.empty()){
        obj.texture = new Texture(texture);
        obj.texture->load();
    }
    if(!normalMap.empty()){
        obj.normalMap = new Texture(normalMap);
        obj.normalMap->load();
    }
    if(!highMap.empty()){
        obj.highMap = new Texture(highMap);
        obj.highMap->load();
    }
    
    if(shapes.find(obj.type) == shapes.end()){
        Object_t tmp = generateSphere(glm::vec3(0, 0, 0), 1, tesselation, colour);
        
        obj.loadData("in_Vertex", tmp.vertices, tmp.vertexLength);
        obj.loadData("in_Colour", tmp.colours, tmp.colourLength);
        obj.loadData("in_TexCoord", tmp.texCoord, tmp.texCoordLength);
        obj.loadData("in_Normal", tmp.normals, tmp.normalLength);
        obj.loadData("in_Tangent", tmp.tangents, tmp.tangentLength);
        obj.loadData("in_BiTangent", tmp.bitangents, tmp.bitangentLength);
        obj.loadIndecies(tmp.indicies);
    }
    
    return obj;
}