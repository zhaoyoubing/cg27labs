// engine/include/renderpasses/SSAODeferredLightPass.h
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
class SSAODeferredLightPass : public RenderPass
{
public:
    explicit SSAODeferredLightPass() { }
    
    // init function callded once before the render loop
    bool init(RenderContext& context) override;

    // render function called once per frame.
    void execute(RenderContext& context) override;

    const int MAX_LIGHTS = 128;

private:

    CameraComp camera_;
    //LightComp lightDefault_;
    
    GLuint gBuf = 0;
    
    GLuint gPos = 0;
    GLuint gNormal = 0;
    GLuint gAlbedo = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;

    // viewport dimensions
    int width_ = 0;
    int height_ = 0;

    // the shader
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    void setPointLights(RenderContext & ctx);
    
};