// include/render/Texture.cpp

#include "render/Texture.h"

#include <spdlog/spdlog.h>

Texture::Texture( int w, int h, TextureFormat fmt, const unsigned char * data)
    : width(w), height(h), format(fmt)
{
    //GLuint textureID;
    
    // Create OpenGL texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    spdlog::debug("Texture id {}", id);

    // --------------------------------------------------
    // 6. Set texture parameters
    // --------------------------------------------------
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine OpenGL format
    GLenum texFormat = GL_RGB;
    GLint internalFormat = GL_RGBA8;

    switch (format) {
        case TextureFormat::R8:           
                texFormat =  GL_RED; 
                internalFormat = GL_R8;
                break;
        case TextureFormat::RG8:          
                texFormat =  GL_RG; 
                internalFormat = GL_RG8;
                break;
        case TextureFormat::RGB8:         
        case TextureFormat::SRGB8:              
                texFormat = GL_RGB; 
                internalFormat = GL_RGB8;
                break;
        case TextureFormat::RGBA8:        
        case TextureFormat::SRGB8_ALPHA8: 
                texFormat = GL_RGBA; 
                break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture() {
    if (id) {
        glDeleteTextures(1, &id);
    }
}