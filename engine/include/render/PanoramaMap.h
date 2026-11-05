#pragma once

#include <glad/glad.h>
#include <string>

// PanoramaMap class for handling panorama textures
// To be used in the future in Image-based Lighting (IBL)
class PanoramaMap : public EnvironMap {
public:
    virtual ~PanoramaMap() = default;

    virtual void bind(GLuint unit) const override {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    virtual bool load(const std::string& filename) override {
        // Load the panorama texture from file (implementation not shown)
        // Set id_, width_, height_, and format_ accordingly
        return true; // Return true if loading is successful
    }
};
