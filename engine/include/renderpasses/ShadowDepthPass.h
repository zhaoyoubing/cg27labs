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

// Forward rendering using Scene Graph
class ShadowDepthPass : public RenderPass
{
public:
    explicit ShadowDepthPass() { }
    // explicit ForwardPass(std::shared_ptr<GPUPipeline> gpuPipe) { }

    bool init(RenderContext& context) override;
    
    // Called once per frame.
    void execute(RenderContext& context) override;

    const int SHADOW_WIDTH = 1024;
    const int SHADOW_HEIGHT = 1024;

    GLuint shadowFBO;  // the framebuffer 
    GLuint depthTex;   // the depth map texture

private:

    CameraComp camera_;
    LightComp lightDefault_;

    std::shared_ptr<GPUPipeline> depthShader;

    void drawSceneGraph(const RenderContext & ctx, std::unique_ptr<SceneNode>& node, 
                        const glm::mat4& parentMatrix = glm::mat4(1.0), 
                        const glm::mat4& viewMatrix = glm::mat4(1.0), const glm::mat4& projMatrix = glm::mat4(1.0));
    
    
    //Framebuffer framebuffer_;
};