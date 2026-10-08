// include/render/Texture.cpp

#include "render/Texture.h"

#include <spdlog/spdlog.h>

Texture::Texture( int w, int h, TextureFormat fmt, const unsigned char * data)
    : width(w), height(h), format(fmt)
{
    //GLuint textureID;
    
    // Create OpenGL texture
    // [TODO] T2.2 Generate and bind the OpenGL texture ID.
    // Hint: Use glGenTextures() and glBindTexture().


    spdlog::debug("Texture id {}", id);

    // --------------------------------------------------
    // 6. Set texture parameters
    // --------------------------------------------------
    // [TODO] T2.3 Set the texture parameters for wrapping and filtering.
    // Hint: Use glTexParameteri() with GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, 
    // GL_TEXTURE_MIN_FILTER, and GL_TEXTURE_MAG_FILTER.
    // Hint: try to use mipmapping for minification filter (GL_LINEAR_MIPMAP_LINEAR).


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

    // Upload texture data to GPU
    // [TODO] T2.4 Upload the texture data to the GPU using glTexImage2D().
    
    
    // [TODO] T2.5 Generate mipmaps for the texture using glGenerateMipmap().
    

    glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture() {
    if (id) {
        glDeleteTextures(1, &id);
    }
}