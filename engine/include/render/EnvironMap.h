#pragma once

#include <glad/glad.h>
#include <string>

class EnvironMap {
public:
    virtual ~EnvironMap() = default;

    virtual void bind(GLuint unit) const = 0;
    virtual bool load(const std::string& filename) = 0;

protected:
    GLuint mapId_ = 0; // OpenGL texture ID
    int width_ = 0;
    int height_ = 0;
    GLenum format_ = GL_RGBA; // Default format
};