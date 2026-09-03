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
    GLuint id = 0;
    int width = 0;
    int height = 0;
    TextureFormat format;

    Texture(int w, int h, TextureFormat fmt, const unsigned char * data);

    ~Texture();

    void bind(unsigned int unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);

        GLint boundTexture = 0;

        //glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

        //if (boundTexture != 0)
        //    spdlog::debug("Texture bound: {}", boundTexture);
        //else
        //   spdlog::warn("No Texture bound");
            
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint getId() const { return id; }
};
