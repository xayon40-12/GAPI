//
//  Texture.hpp
//  SDLTest
//
//  Created by Nathan Touroux on 05/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <unordered_map>
#include <SDL2/SDL.h>
#undef main
#include <SDL2_image/SDL_image.h>
#include <iostream>

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

struct texSave{
    GLuint id;
    int width;
    int height;
    GLenum format;
    GLenum internalFormat;
};

class Texture{
private:
    static int count;
    static std::unordered_map<std::string, texSave> savedTextures;

    GLuint id;
    int textureIndex;
    std::string imageFile;

    int width;
    int height;
    GLenum format;
    GLenum internalFormat;
    GLuint minFilter, magFilter;
    bool emptyTexture;

    SDL_Surface* verticalReverse(SDL_Surface *sourceImage) const;
    SDL_Surface* horizontalReverse(SDL_Surface *sourceImage) const;
    void glLoadTexture(SDL_Surface* texture);
    SDL_Surface* glGetTexture() const;

public:
    explicit Texture(const std::string &imageFile, GLuint minFilter = GL_NEAREST, GLuint magFilter = GL_NEAREST);
    Texture(int width, int height, GLenum format, GLenum internalFormat, GLuint minFilter = GL_NEAREST, GLuint magFilter = GL_NEAREST);
    Texture(Texture const &toCopy);
    Texture& operator=(Texture const &toCopy);
    ~Texture();

    bool load();
    void setImageFile(const std::string &imageFile);

    void savePNG(const std::string &file);//extention .png is automatic don't put it

    void verticalReverse();
    void horizontalReverse();

    void use(int textureIndex);//bind the texture to the shader, call it only once until you use remove()
    void remove();//when finish using the texture call remove() to unbind the texture from the shader

    void setFilter(GLuint minFilter, GLuint magFilter);//GL_NEAREST   GL_LINEAR

    GLuint getID() const;
    int getWidth();
    int getHeight();
};

#endif /* Texture_hpp */
