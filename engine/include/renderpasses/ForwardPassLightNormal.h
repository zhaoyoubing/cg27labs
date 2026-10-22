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
class ForwardPassLightNormal : public RenderPass
{
public:
    explicit ForwardPassLightNormal() { }
    // explicit ForwardPass(std::shared_ptr<GPUPipeline> gpuPipe) { }

    // Called once per frame.
    void execute(RenderContext& context) override;

private:

    // GPUPipeline * pPipe = nullptr;

    CameraComp camera_;
    LightComp lightDefault_;


    void drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                        const glm::mat4& parentMatrix, 
                        const RenderContext & ctx,
                        const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
                        
    //Framebuffer framebuffer_;
};