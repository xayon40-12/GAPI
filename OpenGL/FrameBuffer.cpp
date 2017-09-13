//
//  FrameBuffer.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 16/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "FrameBuffer.hpp"


FrameBuffer::FrameBuffer(int width, int height, int nbColourBuffers, bool useStencilBuffer, GLuint minFilter, GLuint magFilter): id(0), width(width), height(height), depthBuffer(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, minFilter, magFilter), nbColourBuffers(nbColourBuffers), depthBufferID(0), useStencilBuffer(useStencilBuffer), minFilter(minFilter), magFilter(magFilter){
    memset(viewport, 0, sizeof(viewport));
}
FrameBuffer::FrameBuffer(FrameBuffer const &toCopy): id(0), width(toCopy.width), height(toCopy.height), depthBuffer(toCopy.depthBuffer), nbColourBuffers(toCopy.nbColourBuffers), depthBufferID(0), useStencilBuffer(toCopy.useStencilBuffer), minFilter(toCopy.minFilter), magFilter(toCopy.magFilter){
    create();
    memset(viewport, 0, sizeof(viewport));
}
FrameBuffer& FrameBuffer::operator=(FrameBuffer const &toCopy){
    id = 0;
    width = toCopy.width;
    height = toCopy.height;
    depthBuffer = toCopy.depthBuffer;
    nbColourBuffers = toCopy.nbColourBuffers;
    depthBufferID = 0;
    useStencilBuffer = toCopy.useStencilBuffer;
    minFilter = toCopy.minFilter;
    magFilter = toCopy.magFilter;
    create();
    return *this;
}
FrameBuffer::~FrameBuffer(){
    glDeleteFramebuffers(1, &id);
    glDeleteRenderbuffers(1, &depthBufferID);
    colourBuffers.clear();
}

bool FrameBuffer::create(){
    if(glIsFramebuffer(id) == GL_TRUE){
        glDeleteFramebuffers(1, &id);
        colourBuffers.clear();
    }
    
    Texture colourBuffer(width, height, GL_RGBA, GL_RGBA, minFilter, magFilter);
    for(int i = 0;i<nbColourBuffers;i++){
        colourBuffers.push_back(colourBuffer);
    }
    depthBuffer.load();
    
    glGenFramebuffers(1, &id);
    
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    
    GLenum drawBuffers[nbColourBuffers];
    for(int i = 0;i<nbColourBuffers;i++){
        glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0+i), GL_TEXTURE_2D, colourBuffers[i].getID(), 0);
        drawBuffers[i] = (GLenum)(GL_COLOR_ATTACHMENT0+i);
    }
    glDrawBuffers(nbColourBuffers, drawBuffers);
    
    if(useStencilBuffer)
        createRenderBuffer(depthBufferID, GL_DEPTH24_STENCIL8);
    else
        createRenderBuffer(depthBufferID, GL_DEPTH_COMPONENT24);
    
    if(useStencilBuffer)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    else
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer.getID(), 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        glDeleteFramebuffers(1, &id);
        glDeleteRenderbuffers(1, &depthBufferID);
        colourBuffers.clear();
        
        std::cout << "ERROR : FBO can't be created!" << std::endl;
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return true;
}

void FrameBuffer::use(){
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, width, height);
}
void FrameBuffer::remove(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if(viewport[3] != 0)
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

GLuint FrameBuffer::getID() const{
    return id;
}
GLuint FrameBuffer::getColorBufferID(unsigned int index) const{
    return colourBuffers[index].getID();
}
Texture& FrameBuffer::getColorBuffer(unsigned int index){
    return colourBuffers[index];
}
Texture& FrameBuffer::getDepthBuffer(){
    return depthBuffer;
}
int FrameBuffer::getNbColourBuffer(){
    return nbColourBuffers;
}
int FrameBuffer::getWidth() const{
    return width;
}
int FrameBuffer::getHeight() const{
    return height;
}

void FrameBuffer::createRenderBuffer(GLuint &id, GLenum internalFormat){
    if(glIsRenderbuffer(id) == GL_TRUE)
        glDeleteRenderbuffers(1, &id);
    glGenRenderbuffers(1, &id);
    
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}