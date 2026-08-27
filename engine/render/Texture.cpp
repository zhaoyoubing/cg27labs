// include/render/Texture.cpp

#include "render/Texture.h"

Texture::Texture(const unsigned char* imageBytes, int w, int h, int channels)
    : width(w), height(h) 
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Determine format based on channel count
    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;
    else if (channels == 1) format = GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageBytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set standard parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() {
    if (id) {
        glDeleteTextures(1, &id);
    }
}