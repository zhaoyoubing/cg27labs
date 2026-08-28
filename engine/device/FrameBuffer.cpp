#include "device/Framebuffer.h"

#include <iostream>

Framebuffer::Framebuffer(uint32_t width, uint32_t height, FramebufferType type)
    : width(width), height(height), type(type) {
    invalidate();
}

Framebuffer::~Framebuffer() {
    if (rendererID) {
        glDeleteFramebuffers(1, &rendererID);
        if (colorAttachment) glDeleteTextures(1, &colorAttachment);
        if (depthAttachment) glDeleteTextures(1, &depthAttachment);
    }
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : rendererID(other.rendererID),
      colorAttachment(other.colorAttachment),
      depthAttachment(other.depthAttachment),
      width(other.width),
      height(other.height),
      type(other.type) {
    other.rendererID = 0;
    other.colorAttachment = 0;
    other.depthAttachment = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
        if (rendererID) {
            glDeleteFramebuffers(1, &rendererID);
            if (colorAttachment) glDeleteTextures(1, &colorAttachment);
            if (depthAttachment) glDeleteTextures(1, &depthAttachment);
        }

        rendererID = other.rendererID;
        colorAttachment = other.colorAttachment;
        depthAttachment = other.depthAttachment;
        width = other.width;
        height = other.height;
        type = other.type;

        other.rendererID = 0;
        other.colorAttachment = 0;
        other.depthAttachment = 0;
    }
    return *this;
}

void Framebuffer::invalidate() {
    if (rendererID) {
        glDeleteFramebuffers(1, &rendererID);
        if (colorAttachment) glDeleteTextures(1, &colorAttachment);
        if (depthAttachment) glDeleteTextures(1, &depthAttachment);
        rendererID = 0;
        colorAttachment = 0;
        depthAttachment = 0;
    }

    glGenFramebuffers(1, &rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

    if (type == FramebufferType::ColorDepth) {
        // Create Color Texture
        glGenTextures(1, &colorAttachment);
        glBindTexture(GL_TEXTURE_2D, colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

        // Create Depth/Stencil Texture
        glGenTextures(1, &depthAttachment);
        glBindTexture(GL_TEXTURE_2D, depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
    } 
    else if (type == FramebufferType::DepthOnly) {
        // Create Depth-Only Texture (Ideal for Shadow Mapping)
        glGenTextures(1, &depthAttachment);
        glBindTexture(GL_TEXTURE_2D, depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderValue);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
        
        // Explicitly tell OpenGL we are not writing to any color buffers
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(uint32_t newWidth, uint32_t newHeight) {
    if (newWidth == 0 || newHeight == 0 || (newWidth == width && newHeight == height)) {
        return;
    }

    width = newWidth;
    height = newHeight;
    invalidate();
}