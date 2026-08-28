#pragma once
#include <glad/glad.h>
#include <memory>
#include <cstdint>

enum class FramebufferType {
    ColorDepth,
    DepthOnly
};

class Framebuffer {
public:
    Framebuffer(uint32_t width, uint32_t height, FramebufferType type = FramebufferType::ColorDepth);
    ~Framebuffer();

    // Prevent copying to avoid double-freeing OpenGL resources
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    // Allow moving
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    void bind() const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    uint32_t getColorAttachmentRendererID() const { return colorAttachment; }
    uint32_t getDepthAttachmentRendererID() const { return depthAttachment; }
    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }

private:
    void invalidate();

    uint32_t rendererID = 0;
    uint32_t colorAttachment = 0;
    uint32_t depthAttachment = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    FramebufferType type;
};