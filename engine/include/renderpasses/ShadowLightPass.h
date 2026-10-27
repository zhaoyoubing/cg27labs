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
class ShadowLightPass : public RenderPass
{
public:
    explicit ShadowLightPass() { }
    
    bool init(RenderContext& context) override;

    // Called once per frame.
    void execute(RenderContext& context) override;

    /*
    void setShadowMap(GLuint map) { 
        shadowMap = map;
        bUseShadow = glIsTexture(shadowMap);
    }
    */

private:

    CameraComp camera_;
    LightComp lightDefault_;
    
    bool bInited;

    //bool bUseShadow = false;
    //GLuint shadowMap;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    void drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         RenderContext& context,
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    
};