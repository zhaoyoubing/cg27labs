// engine/include/renderpasses/ForwardPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"
#include "entity/LightComp.h"

// GBuffer Pass for Deferred Rendering
class BloomScenePass : public RenderPass
{
public:
    explicit BloomScenePass() { }
    
    bool init(RenderContext& context) override;

    // Called once per frame.
    void execute(RenderContext& context) override;

private:

    CameraComp camera_;
    LightComp lightDefault_ = {
        .type = POINT,
        .pos = glm::vec3(3.0f),
        .dir = glm::vec3(0.0f, -1.0f, 0.0f),
        .colour = glm::vec3(1.0f, 1.0f, 1.0f),
        .intensity = 1.0f,
        .a = 1.0f,
        .b = 0.09f,
        .c = 0.032f 
    };


    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    // GBuffer dimensions
    int width_ = 0;
    int height_ = 0;

    GLuint FBO = 0;  // frame buffer object
    GLuint texScene = 0;  // scene rendering
    GLuint depthBuf = 0;

    void drawSceneGraph(RenderContext& context, std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    
};