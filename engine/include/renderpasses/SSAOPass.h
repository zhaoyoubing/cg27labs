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

#include "device/GPULight.h"

// GBuffer Pass for Deferred Rendering
class SSAOPass : public RenderPass
{
public:
    explicit SSAOPass() { }
    
    bool init(RenderContext& context) override;

    void execute(RenderContext& context) override;

private:

    CameraComp camera_;
    LightComp lightDefault_;
    
    GLuint FBO = 0;  // frame buffer object
    GLuint texSSAO = 0; // SSAO output texture

    GLuint gNormal = 0;
    GLuint gAlbedo = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;

    // GBuffer dimensions
    int width_ = 0;
    int height_ = 0;


    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

};