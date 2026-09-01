// include/render/Texture.h
#pragma once

#include <glad/glad.h>
#include <string>

using TextureHandle = uint32_t;

struct Texture {
    GLuint id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
    std::string path;

    Texture(const unsigned char* imageBytes, int w, int h, int channels);
    ~Texture();

    void bind(unsigned int unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    GLuint getId() const { return id; }
};
