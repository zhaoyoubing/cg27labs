#pragma once

class RenderPass {
public:
    virtual ~RenderPass() = default;

    virtual void init() = 0;
    virtual void execute(RenderContext& context) = 0;
};

/*
class RenderPass {
public:
    struct Spec {
        std::shared_ptr<Framebuffer> targetFramebuffer;
        bool clearColor = true;
        bool clearDepth = true;
        glm::vec4 clearColorValue = {0.1f, 0.1f, 0.1f, 1.0f};
    };

    RenderPass(const Spec& spec);
    void begin();
    void end();

private:
    Spec m_Spec;
};
*/