//
//  FrameBuffer.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 16/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

#include <iostream>
#include <vector>
#include "Texture.hpp"

class FrameBuffer
{
private:
    GLuint id;
    int width;
    int height;
    std::vector<Texture> colourBuffers;
    Texture depthBuffer;
    int nbColourBuffers;
    GLuint depthBufferID;
    bool useStencilBuffer;
    
    GLint viewport[4];
    
    GLuint minFilter, magFilter;
    
    void createRenderBuffer(GLuint &id, GLenum internalFormat);
public:
    FrameBuffer(int width, int height, int nbColourBuffers = 1, bool useStencilBuffer = false, GLuint minFilter = GL_NEAREST, GLuint magFilter = GL_NEAREST);
    FrameBuffer(FrameBuffer const &toCopy);
    FrameBuffer& operator=(FrameBuffer const &toCopy);
    ~FrameBuffer();
    
    bool create();//initialise and generate the frameBuffer, needed before using frameBuffer
    
    void use();//call before draw inside the frameBuffer
    void remove();//call when drawing on the frameBuffer is finished
    
    GLuint getID() const;
    GLuint getColorBufferID(unsigned int index = 0) const;
    Texture& getColorBuffer(unsigned int index = 0);
    Texture& getDepthBuffer();
    int getNbColourBuffer();
    int getWidth() const;
    int getHeight() const;
};

#endif /* FrameBuffer_hpp */
