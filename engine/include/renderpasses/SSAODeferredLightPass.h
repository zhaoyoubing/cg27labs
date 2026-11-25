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

// SSAO Deferred Lighting Pass
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

    // viewport dimensions
    int width_ = 0;
    int height_ = 0;

    // the shader
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    void setPointLights(RenderContext & ctx);
    
};