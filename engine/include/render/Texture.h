// include/render/Texture.h
#pragma once

#include <glad/glad.h>
#include <string>

#include <spdlog/spdlog.h>

//using TextureHandle = uint32_t;

enum class TextureFormat {
// 8-bit Linear (For non-color data: Roughness, Metallic, AO, Normal maps)
    R8,
    RG8,
    RGB8,
    RGBA8,

    // 8-bit sRGB (For color data: Base Color / Albedo maps)
    SRGB8,
    SRGB8_ALPHA8,

    // High Dynamic Range / Floating-Point (For HDR environment maps, lighting data)
    RGB16F,
    RGBA16F,
    RGB32F,
    RGBA32F
};

struct Texture {
    GLuint id = 0; // OpenGL texture ID

    int width = 0;
    int height = 0;
    TextureFormat format;

    Texture(int w, int h, TextureFormat fmt, const unsigned char * data);

    ~Texture();

    // bind the current texture to a specific texture unit (default is 0)
    void bind(unsigned int unit = 0) const {
        // [TODO] T2.1 Implement the bind() method to bind the current texture to a specific texture unit.
        // Hint: Use glActiveTexture() and glBindTexture().
        // Hint: The default texture unit is GL_TEXTURE0, and you can add the unit index to it to bind to different texture units.
        // Hint: The current texture has an OpenGL texture ID stored in the 'id' member variable.

            
    }

    // unbind the current texture
    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // return the OpenGL texture ID
    GLuint getId() const { return id; }
};
