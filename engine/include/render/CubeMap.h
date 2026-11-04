#pragma once

#include <glad/glad.h>
#include <string>

class CubeMap : public EnvironMap {
public:
    virtual ~CubeMap() = default;

    virtual void bind(GLuint unit) const override {  
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
    }
    
    virtual bool load(const std::string& filename) override {
        // Load the cube map texture from file (implementation not shown)
        // Set id_, width_, height_, and format_ accordingly
        return true; // Return true if loading is successful
    }
};