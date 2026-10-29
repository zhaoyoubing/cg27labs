#pragma once

#include <glad/glad.h>
#include <stdexcept>
#include <string>

struct GBuffer
{
    GLuint fbo = 0;
    GLuint albedo = 0;
    GLuint normal = 0;
    GLuint material = 0;
    GLuint depth = 0;

    int width = 0;
    int height = 0;

    void create(int w, int h)
    {
        if (w <= 0 || h <= 0)
            throw std::runtime_error("Invalid G-buffer size");

        width = w;
        height = h;

        // Create framebuffer
        glCreateFramebuffers(1, &fbo);

        // Helper: create 2D texture with immutable storage
        auto createTexture = [&](GLenum format) -> GLuint
        {
            GLuint texture = 0;

            glCreateTextures(GL_TEXTURE_2D, 1, &texture);

            glTextureStorage2D(
                texture, 1, format, width, height
            );

            glTextureParameteri(
                texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST
            );
            glTextureParameteri(
                texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST
            );

            glTextureParameteri(
                texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
            );
            glTextureParameteri(
                texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
            );

            return texture;
        };

        // Create G-buffer textures
        albedo   = createTexture(GL_RGBA8);
        normal   = createTexture(GL_RGBA16F);
        material = createTexture(GL_RGBA8);
        depth    = createTexture(GL_DEPTH_COMPONENT24);

        // Attach colour textures
        glNamedFramebufferTexture(
            fbo, GL_COLOR_ATTACHMENT0, albedo, 0
        );

        glNamedFramebufferTexture(
            fbo, GL_COLOR_ATTACHMENT1, normal, 0
        );

        glNamedFramebufferTexture(
            fbo, GL_COLOR_ATTACHMENT2, material, 0
        );

        // Attach depth texture
        glNamedFramebufferTexture(
            fbo, GL_DEPTH_ATTACHMENT, depth, 0
        );

        // Specify fragment shader output attachments
        constexpr GLenum drawBuffers[] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2
        };

        glNamedFramebufferDrawBuffers(
            fbo, 3, drawBuffers
        );

        // Check framebuffer completeness
        GLenum status = glCheckNamedFramebufferStatus(
            fbo, GL_FRAMEBUFFER
        );

        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error(
                "G-buffer incomplete: " +
                std::to_string(status)
            );
        }
    }

    void bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }

    void destroy()
    {
        glDeleteTextures(1, &albedo);
        glDeleteTextures(1, &normal);
        glDeleteTextures(1, &material);
        glDeleteTextures(1, &depth);

        glDeleteFramebuffers(1, &fbo);

        fbo = albedo = normal = material = depth = 0;
    }
};