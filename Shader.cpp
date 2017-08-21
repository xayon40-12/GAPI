//
//  Shader.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 01/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Shader.hpp"

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif

int Shader::count = 0;

std::map<std::string, GLuint> Shader::vertexIDs;
std::map<std::string, GLuint> Shader::geometryIDs;
std::map<std::string, GLuint> Shader::fragmentIDs;

Shader::Shader(std::string sourceVertex, std::string sourceGeometry, std::string sourceFragment): vertexID(0), geometryID(0), fragmentID(0), programID(0), sourceVertex(sourceVertex), sourceGeometry(sourceGeometry), sourceFragment(sourceFragment), vboID(0), iboID(0), vaoID(0), useIndicies(false), nbElements(0){
    count++;
}
Shader::Shader(std::string sourceVertex, std::string sourceFragment): vertexID(0), geometryID(0), fragmentID(0), programID(0), sourceVertex(sourceVertex), sourceGeometry(""), sourceFragment(sourceFragment), vboID(0), iboID(0), vaoID(0), useIndicies(false), nbElements(0){
    count++;
}
Shader::Shader(Shader const &toCopy): offset(toCopy.offset), vertexID(toCopy.vertexID), geometryID(toCopy.geometryID), fragmentID(toCopy.fragmentID), programID(0), sourceVertex(toCopy.sourceVertex), sourceGeometry(toCopy.sourceGeometry), sourceFragment(toCopy.sourceFragment), vboID(0), iboID(0), vaoID(0), useIndicies(false), nbElements(0), glslData(toCopy.glslData), indicies(toCopy.indicies){
    
    count++;
}
Shader& Shader::operator=(Shader const &toCopy){
    offset = toCopy.offset;
    vertexID = toCopy.vertexID;
    geometryID = toCopy.geometryID;
    fragmentID = toCopy.fragmentID;
    programID = 0;
    sourceVertex = toCopy.sourceVertex;
    sourceGeometry = toCopy.sourceGeometry;
    sourceFragment = toCopy.sourceFragment;
    vboID = 0;
    iboID = 0;
    vaoID = 0;
    glslData = toCopy.glslData;
    indicies = toCopy.indicies;
    useIndicies = false;
    nbElements = 0;
    
    return *this;
}
Shader::~Shader(){
    count--;
    if(count == 0){
        for(auto v : vertexIDs)
            glDeleteShader(v.second);
        for(auto f : geometryIDs)
            glDeleteShader(f.second);
        for(auto f : fragmentIDs)
            glDeleteShader(f.second);
    }
    
    glDeleteProgram(programID);
    
    if(glIsBuffer(vboID) == GL_TRUE)
        glDeleteBuffers(1, &vboID);
    if(glIsBuffer(iboID) == GL_TRUE)
        glDeleteBuffers(1, &iboID);
    if(glIsVertexArray(vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &vaoID);
}

bool Shader::create(){
    if(sourceVertex == "" || sourceFragment == "") return false;
    
    std::map<std::string, GLuint>::iterator it;
    if((it = vertexIDs.find(sourceVertex)) == vertexIDs.end()){
        if(!compileShader(vertexID, GL_VERTEX_SHADER, sourceVertex))
            return false;
        else
            vertexIDs[sourceVertex] = vertexID;
    }else{
        vertexID = it->second;
    }
    if(sourceGeometry != ""){
        if((it = geometryIDs.find(sourceGeometry)) == geometryIDs.end()){
            if(!compileShader(geometryID, GL_GEOMETRY_SHADER, sourceGeometry))
                return false;
            else
                geometryIDs[sourceGeometry] = geometryID;
        }else{
            geometryID = it->second;
        }
    }
    if((it = fragmentIDs.find(sourceFragment)) == fragmentIDs.end()){
        if(!compileShader(fragmentID, GL_FRAGMENT_SHADER, sourceFragment))
            return false;
        else
            fragmentIDs[sourceFragment] = fragmentID;
    }else{
        fragmentID = it->second;
    }
    
    return reload();
}
bool Shader::linkProgram(){
    if(glIsProgram(programID) == GL_TRUE)
        glDeleteProgram(programID);
    programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    if(sourceGeometry != "") glAttachShader(programID, geometryID);
    glAttachShader(programID, fragmentID);
    for(int i = 0;i<glslData.size();i++)
        glBindAttribLocation(programID, i, glslData[i].name.c_str());
    glLinkProgram(programID);
    GLint erreurLink(0);
    glGetProgramiv(programID, GL_LINK_STATUS, &erreurLink);
    if(erreurLink != GL_TRUE){
        GLint tailleErreur(0);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &tailleErreur);
        char *erreur = new char[tailleErreur+1];
        glGetProgramInfoLog(programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';
        std::cout << erreur << std::endl;
        delete[] erreur;
        glDeleteProgram(programID);
        return false;
    }else
        return true;
}
bool Shader::reload(){
    auto tmp = glslData;
    glslData.clear();
    for(auto &d : tmp)
        if(d.dataArray.size() != 0)
            glslData.push_back(d);
    if(glslData.size() == 0) return false;
    
    if(linkProgram()){
        if(glIsBuffer(vboID) == GL_TRUE)
            glDeleteBuffers(1, &vboID);
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        
        offset.clear();
        int cum[glslData.size()+1];
        cum[0] = 0;
        for(int i = 0;i<glslData.size();i++){
            offset[glslData[i].name] = cum[i];
            cum[i+1] = cum[i] + (int)glslData[i].dataArray.size()*sizeof(float);
        }
        
        glBufferData(GL_ARRAY_BUFFER, cum[glslData.size()], 0, GL_STATIC_DRAW);
        for(int i = 0;i<glslData.size();i++)
            glBufferSubData(GL_ARRAY_BUFFER, cum[i], glslData[i].dataArray.size()*sizeof(float), glslData[i].dataArray.data());
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        if(indicies.size() != 0){
            if(glIsBuffer(iboID) == GL_TRUE)
                glDeleteBuffers(1, &iboID);
            glGenBuffers(1, &iboID);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        
        if(glIsVertexArray(vaoID) == GL_TRUE)
            glDeleteVertexArrays(1, &vaoID);
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
        
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        for(int i = 0;i<glslData.size();i++){
            glVertexAttribPointer(i, glslData[i].dataLength, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(cum[i]));
            glEnableVertexAttribArray(i);
        }
        
        if(indicies.size() != 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(0);
        
        if(indicies.size() == 0){
            useIndicies = false;
            nbElements = (unsigned int)glslData[0].dataArray.size()/glslData[0].dataLength;
        }else{
            useIndicies = true;
            nbElements = (unsigned int)indicies.size();
        }
        
        return true;
    }else
        return false;
}
bool Shader::compileShader(GLuint &shader, GLenum type, std::string const &sourceFile){
    shader = glCreateShader(type);
    if(shader == 0){
        std::cout << "ERREUR, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }
    std::ifstream fichier(sourceFile.c_str());
    if(!fichier){
        std::cout << "ERREUR le fichier " << sourceFile << " est introuvable" << std::endl;
        glDeleteShader(shader);
        return false;
    }
    std::string ligne, codeSource;
    
    while(std::getline(fichier, ligne))
        codeSource+=ligne + '\n';
    fichier.close();
    const GLchar* chaineCodeSource = codeSource.c_str();
    glShaderSource(shader, 1, &chaineCodeSource, 0);
    glCompileShader(shader);
    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);
    if(erreurCompilation != GL_TRUE){
        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);
        char *erreur = new char[tailleErreur+1];
        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';
        std::cout << erreur << std::endl;
        delete[] erreur;
        glDeleteShader(shader);
        return false;
    }
    else
        return true;
}
void Shader::use(){
    glUseProgram(programID);
    glBindVertexArray(vaoID);
}
void Shader::remove(){
    glBindVertexArray(0);
    glUseProgram(0);
}
void Shader::draw(GLenum mode, GLint first, GLsizei count){
    glDrawArrays(mode, first, count);
}
void Shader::draw(GLenum mode){
    if(useIndicies)
        glDrawElements(mode, nbElements, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(mode, 0, nbElements);
}
void Shader::drawElements(GLenum mode, std::vector<GLuint> indicies){
    glDrawElements(mode, (int)indicies.size(), GL_UNSIGNED_INT, indicies.data());
}

void Shader::loadData(std::string glslName, std::vector<float> const &data, int dataLength){
    int index = -1;
    for(int i = 0;i<glslData.size();i++)
        if(glslData[i].name == glslName){
            index = i;
            break;
        }
    
    if(index != -1){
        glslData[index].dataArray = data;
        glslData[index].dataLength = dataLength;
        glslData[index].size = data.size();
        
    }else
        glslData.push_back({glslName, data, dataLength, data.size()});
}
bool Shader::updateData(std::string glslName, std::vector<float> const &data){
    bool updated = false;
    for(auto &d : glslData)
        if(d.name == glslName)
            if(d.size == data.size()){
                updateVBO(glslName, data);
                updated = true;
                break;
            }
    
    return updated;
}
void Shader::loadIndecies(std::vector<GLuint> indicies){
    this->indicies = indicies;
}

void Shader::clearBuffers(){
    for(auto &d : glslData)
        d.dataArray.clear();
    indicies.clear();
}

void Shader::uniformData(std::string name, int data){
    glUniform1i(uniformLocation(name), data);
}
void Shader::uniformData(std::string name, float data){
    glUniform1f(uniformLocation(name), data);
}
void Shader::uniformData(std::string name, double data){
    glUniform1d(uniformLocation(name), data);
}
void Shader::uniformData(std::string name, std::vector<int> data, int elementLength){
    if(elementLength == 1) glUniform1iv(uniformLocation(name), (int)data.size(), data.data());
    if(elementLength == 2) glUniform2iv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 3) glUniform3iv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 4) glUniform4iv(uniformLocation(name), (int)data.size()/elementLength, data.data());
}
void Shader::uniformData(std::string name, std::vector<float> data, int elementLength){
    if(elementLength == 1) glUniform1fv(uniformLocation(name), (int)data.size(), data.data());
    if(elementLength == 2) glUniform2fv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 3) glUniform3fv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 4) glUniform4fv(uniformLocation(name), (int)data.size()/elementLength, data.data());
}
void Shader::uniformData(std::string name, std::vector<double> data, int elementLength){
    if(elementLength == 1) glUniform1dv(uniformLocation(name), (int)data.size(), data.data());
    if(elementLength == 2) glUniform2dv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 3) glUniform3dv(uniformLocation(name), (int)data.size()/elementLength, data.data());
    if(elementLength == 4) glUniform4dv(uniformLocation(name), (int)data.size()/elementLength, data.data());
}
void Shader::uniformData(std::string name, glm::vec2 data){
    glUniform2f(uniformLocation(name), data.x, data.y);
}
void Shader::uniformData(std::string name, glm::vec3 data){
    glUniform3f(uniformLocation(name), data.x, data.y, data.z);
}
void Shader::uniformData(std::string name, glm::vec4 data){
    glUniform4f(uniformLocation(name), data.x, data.y, data.z, data.w);
}
void Shader::uniformData(std::string name, std::vector<glm::vec2> data){
    std::vector<float> tab;
    for(glm::vec2 v : data){
        tab.push_back(v.x);
        tab.push_back(v.y);
    }
    glUniform2fv(uniformLocation(name), (int)tab.size(), tab.data());
}
void Shader::uniformData(std::string name, std::vector<glm::vec3> data){
    std::vector<float> tab;
    for(glm::vec3 v : data){
        tab.push_back(v.x);
        tab.push_back(v.y);
        tab.push_back(v.z);
    }
    glUniform2fv(uniformLocation(name), (int)tab.size(), tab.data());
}
void Shader::uniformData(std::string name, std::vector<glm::vec4> data){
    std::vector<float> tab;
    for(glm::vec4 v : data){
        tab.push_back(v.x);
        tab.push_back(v.y);
        tab.push_back(v.z);
        tab.push_back(v.w);
    }
    glUniform2fv(uniformLocation(name), (int)tab.size(), tab.data());
}

void Shader::uniformMatrix(std::string name, glm::mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uniformMatrix(std::string name, glm::mat3 mat){
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uniformMatrix(std::string name, glm::mat2 mat){
    glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::uniformMVP(glm::mat4 MVP){
    glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
}

GLint Shader::uniformLocation(std::string name){
    return glGetUniformLocation(programID, name.c_str());
}

GLuint Shader::getProgram()
{
    return programID;
}
GLuint Shader::getVAO(){
    return vaoID;
}
GLuint Shader::getVBO(){
    return vboID;
}


void Shader::updateVBO(std::string glslName, std::vector<float> const &data)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    
    void *adresseVBO = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if(adresseVBO == NULL)
    {
        std::cout << "ERREUR au niveau de la recuperation du VBO" << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return;
    }
    
    memcpy((char*)adresseVBO+offset[glslName], data.data(), data.size()*sizeof(float));
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    adresseVBO = 0;
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}