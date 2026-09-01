// include/render/Texture.cpp

#include "render/Texture.h"

#include <spdlog/spdlog.h>

Texture::Texture( int w, int h, TextureFormat fmt, const unsigned char * data)
    : width(w), height(h), format(fmt)
{
    GLuint textureID;
    
    // Create OpenGL texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // --------------------------------------------------
    // 6. Set texture parameters
    // --------------------------------------------------
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine OpenGL format
    GLenum texFormat = GL_RGB;

    switch (format) {
        case TextureFormat::R8:           texFormat =  GL_RED; break;
        case TextureFormat::RG8:          texFormat =  GL_RG; break;
        case TextureFormat::RGB8:         
        case TextureFormat::SRGB8:        
        case TextureFormat::RGB16F:       
        case TextureFormat::RGB32F:       texFormat = GL_RGB; break;
        case TextureFormat::RGBA8:        
        case TextureFormat::SRGB8_ALPHA8: 
        case TextureFormat::RGBA16F:      
        case TextureFormat::RGBA32F:      texFormat = GL_RGBA; break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture() {
    if (id) {
        glDeleteTextures(1, &id);
    }
}