#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

// Forward rendering using Scene Graph
class ForwardPass : public RenderPass
{
public:
    explicit ForwardPass() { }
    explicit ForwardPass(std::shared_ptr<GPUPipeline> gpuPipe) { }
    
    // Called once per frame.
    void execute(RenderContext& context) override;

private:
    void drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         const MaterialManager & matMgr,
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    //Framebuffer framebuffer_;
};