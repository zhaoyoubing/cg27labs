// engine/include/renderpasses/SSRReflectPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"

// Screen space reflection pass
class SSRReflectPass : public RenderPass
{
public:
    explicit SSRReflectPass() { }
    
    // called once before the rendering loop
    bool init(RenderContext& context) override;

    // Render function called once per frame.
    void execute(RenderContext& context) override;

private:

    CameraComp camera_;
    
    GLuint gAlbedo = 0;
    GLuint gNormal = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;

    // framebuffer dimensions
    int width_ = 0;
    int height_ = 0;

    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    GLuint FBO = 0;
    GLuint texFBO = 0;
    
};