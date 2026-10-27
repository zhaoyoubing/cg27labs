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
class GBufferPass : public RenderPass
{
public:
    explicit GBufferPass() { }
    
    bool init(RenderContext& context) override;

    // Called once per frame.
    void execute(RenderContext& context) override;

private:

    CameraComp camera_;
    LightComp lightDefault_;
    
    bool bInited;

    GLuint gBuf = 0;
    GLuint gPos = 0;
    GLuint gNormal = 0;
    GLuint gAlbedo = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;

    int width_ = 0;
    int height_ = 0;

    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    void drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         RenderContext& context,
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    
};