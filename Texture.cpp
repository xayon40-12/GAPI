//
//  Texture.cpp
//  SDLTest
//
//  Created by Nathan Touroux on 05/05/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "Texture.hpp"

int Texture::count = 0;
std::map<std::string, texSave> Texture::savedTextures;

Texture::Texture(std::string imageFile, GLuint minFilter, GLuint magFilter): id(0), textureIndex(0), imageFile(imageFile), width(0), height(0), format(0), internalFormat(0), minFilter(minFilter), magFilter(magFilter), emptyTexture(false){
    count++;
}
Texture::Texture(int width, int height, GLenum format, GLenum internalFormat, GLuint minFilter, GLuint magFilter): id(0), textureIndex(0), imageFile(""), width(width), height(height), format(format), internalFormat(internalFormat), minFilter(minFilter), magFilter(magFilter), emptyTexture(true){
    count++;
}
Texture::Texture(Texture const &toCopy): id(0), textureIndex(0), imageFile(toCopy.imageFile), width(toCopy.width), height(toCopy.height), format(toCopy.format), internalFormat(toCopy.internalFormat), minFilter(toCopy.minFilter), magFilter(toCopy.magFilter), emptyTexture(toCopy.emptyTexture){
    count++;
    if(toCopy.emptyTexture)
        glLoadTexture(toCopy.glGetTexture());
    else
        id = toCopy.id;
}
Texture& Texture::operator=(Texture const &toCopy){
    if(emptyTexture)
        glDeleteTextures(1, &id);
    
    if(toCopy.emptyTexture){
        id = 0;
        glLoadTexture(toCopy.glGetTexture());
    }else
        id = toCopy.id;
    textureIndex = 0;
    imageFile = toCopy.imageFile;
    width = toCopy.width;
    height = toCopy.height;
    format = toCopy.format;
    internalFormat = toCopy.internalFormat;
    minFilter = toCopy.minFilter;
    magFilter = toCopy.magFilter;
    emptyTexture = toCopy.emptyTexture;
    
    return *this;
}
Texture::~Texture(){
    if(emptyTexture)
        glDeleteTextures(1, &id);
    count--;
    if(count == 0)
        for(auto &s: savedTextures)
            glDeleteTextures(1, &s.second.id);
}

SDL_Surface* Texture::glGetTexture() const{
    SDL_Surface *image = 0;
    bool found = true;
    Uint32 r = 0, g = 0x0000FF00, b = 0,  a = 0xFF000000;
    int BitsPerPixel = 0;
    switch(format){
        case GL_RGB:
            BitsPerPixel = 24;
            r = 0x000000FF; b = 0x00FF0000;
            break;
        case GL_BGR:
            BitsPerPixel = 24;
            r = 0x00FF0000; b = 0x000000FF;
            break;
        case GL_RGBA:
            BitsPerPixel = 32;
            r = 0x000000FF; b = 0x00FF0000;
            break;
        case GL_BGRA:
            BitsPerPixel = 32;
            r = 0x00FF0000; b = 0x000000FF;
            break;
        default:
            found = false;
            break;
    }
    if(found){
        image = SDL_CreateRGBSurface(0, width, height, BitsPerPixel, r, g, b, a);
        
        glBindTexture(GL_TEXTURE_2D, id);
        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, image->pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    return image;
}
void Texture::glLoadTexture(SDL_Surface* texture){
    if(glIsTexture(id) == GL_TRUE)
        glDeleteTextures(1, &id);
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    if(texture != 0)
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->w, texture->h, 0, format, GL_UNSIGNED_BYTE, texture->pixels);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}
bool Texture::load(){
    if(emptyTexture){
        glLoadTexture(0);
    }else{
        if(savedTextures.find(imageFile) == savedTextures.end()){
            SDL_Surface *reversedImage;
            SDL_Surface *imageSDL = IMG_Load(imageFile.c_str());
            
            if(imageSDL == 0){
                std::cout << "Error : " << SDL_GetError() << std::endl;
                return false;
            }
            
            reversedImage = verticalReverse(imageSDL);
            SDL_FreeSurface(imageSDL);
            
            if(reversedImage->format->BytesPerPixel == 3){
                internalFormat = GL_RGB;
                if(reversedImage->format->Rmask == 0xff){
                    format = GL_RGB;
                }else{
                    format = GL_BGR;
                }
            }else if(reversedImage->format->BytesPerPixel == 4){
                internalFormat = GL_RGBA;
                if(reversedImage->format->Rmask == 0xff){
                    format = GL_RGBA;
                }else{
                    format = GL_BGRA;
                }
            }else{
                std::cout << "Erreur, format de l'image inconnut" << std::endl;
                return false;
            }
            width = reversedImage->w;
            height = reversedImage->h;
            
            glLoadTexture(reversedImage);
            SDL_FreeSurface(reversedImage);
            
            savedTextures[imageFile] = {id, width, height, format, internalFormat};
        }else{
            texSave save = savedTextures[imageFile];
            id = save.id;
            width = save.width;
            height = save.height;
            format = save.format;
            internalFormat = save.internalFormat;
        }
    }
    
    return true;
}

void Texture::setImageFile(const std::string &imageFile){
    this->imageFile =  imageFile;
}
void Texture::savePNG(std::string file){
    SDL_Surface *texture = glGetTexture();
    if(texture != 0){
        IMG_SavePNG(verticalReverse(texture),(file+".png").c_str());
    }
}
SDL_Surface* Texture::verticalReverse(SDL_Surface *sourceImage) const{
    // Copie conforme de l'image source sans les pixels
    
    SDL_Surface *reversedImage = SDL_CreateRGBSurface(0, sourceImage->w, sourceImage->h, sourceImage->format->BitsPerPixel, sourceImage->format->Rmask, sourceImage->format->Gmask, sourceImage->format->Bmask, sourceImage->format->Amask);
    
    
    // Tableau intermÈdiaires permettant de manipuler les pixels
    
    unsigned char* pixelsSources = (unsigned char*) sourceImage->pixels;
    unsigned char* pixelsInverses = (unsigned char*) reversedImage->pixels;
    
    
    // Inversion des pixels
    
    for(int i = 0; i < sourceImage->h; i++){
        for(int j = 0; j < sourceImage->w * sourceImage->format->BytesPerPixel; j++)
            pixelsInverses[(sourceImage->w * sourceImage->format->BytesPerPixel * (sourceImage->h - 1 - i)) + j] = pixelsSources[(sourceImage->w * sourceImage->format->BytesPerPixel * i) + j];
    }
    
    
    // Retour de l'image inversÈe
    
    return reversedImage;
}
SDL_Surface* Texture::horizontalReverse(SDL_Surface *sourceImage) const{
    // Copie conforme de l'image source sans les pixels
    
    SDL_Surface *reversedImage = SDL_CreateRGBSurface(0, sourceImage->w, sourceImage->h, sourceImage->format->BitsPerPixel, sourceImage->format->Rmask, sourceImage->format->Gmask, sourceImage->format->Bmask, sourceImage->format->Amask);
    
    
    // Tableau intermÈdiaires permettant de manipuler les pixels
    
    unsigned char* pixelsSources = (unsigned char*) sourceImage->pixels;
    unsigned char* pixelsInverses = (unsigned char*) reversedImage->pixels;
    
    
    // Inversion des pixels
    
    for(int i = 0; i < sourceImage->h; i++){
        for(int j = 0; j < sourceImage->w * sourceImage->format->BytesPerPixel; j+=sourceImage->format->BytesPerPixel)
            for(int k = 0;k<sourceImage->format->BytesPerPixel;k++)
            pixelsInverses[(sourceImage->w * sourceImage->format->BytesPerPixel * i) + (sourceImage->w * sourceImage->format->BytesPerPixel -sourceImage->format->BytesPerPixel -j +k)] = pixelsSources[(sourceImage->w * sourceImage->format->BytesPerPixel * i) + j + k];
    }
    
    
    // Retour de l'image inversÈe
    
    return reversedImage;
}
void Texture::verticalReverse(){
    SDL_Surface *texture = glGetTexture();
    if(texture != 0){
        SDL_Surface *reversedImage = verticalReverse(texture);
        glLoadTexture(reversedImage);
        SDL_FreeSurface(texture);
        SDL_FreeSurface(reversedImage);
    }
}
void Texture::horizontalReverse(){
    SDL_Surface *texture = glGetTexture();
    if(texture != 0){
        SDL_Surface *reversedImage = horizontalReverse(texture);
        glLoadTexture(reversedImage);
        SDL_FreeSurface(texture);
        SDL_FreeSurface(reversedImage);
    }
}

void Texture::use(int textureIndex){
    this->textureIndex = textureIndex;
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::remove(){
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFilter(GLuint minFilter, GLuint magFilter){
    glBindTexture(GL_TEXTURE_2D, id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() const{
    return id;
}
int Texture::getWidth(){
    return width;
}
int Texture::getHeight(){
    return height;
}