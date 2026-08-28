#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "systems/RenderSystem.h"
#include "renderPasses/Framebuffer.h"
#include "renderPasses/RenderPass.h"
#include "renderPasses/PipelineState.h"
#include "device/Shader.h"

class SceneRenderer {
public:
    SceneRenderer(uint32_t width, uint32_t height);
    ~SceneRenderer() = default;

    void onResize(uint32_t width, uint32_t height);
    void renderFrame(entt::registry& registry);

private:
    // 1. Core Execution Engine
    RenderSystem renderSystem;

    // 2. Resources, Passes, and Pipelines (Initialized once)
    std::shared_ptr<Framebuffer> shadowFramebuffer;
    std::unique_ptr<RenderPass>  shadowPass;
    std::unique_ptr<PipelineState> shadowPipeline;

    std::unique_ptr<RenderPass>  mainPass;
    std::unique_ptr<PipelineState> pbrPipeline;

    // Cached Shaders
    std::shared_ptr<Shader> shadowShader;
    std::shared_ptr<Shader> pbrShader;
};