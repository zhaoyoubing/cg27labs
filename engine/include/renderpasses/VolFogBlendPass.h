// engine/include/renderpasses/VolFogBlendPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"

// Volumetric Fog Blend Pass
class VolFogBlendPass : public RenderPass
{
public:
    explicit VolFogBlendPass() { }
    
    // called once before the rendering loop
    bool init(RenderContext& context) override;

     // Render function called once per frame.
    void execute(RenderContext& context) override;

private:
    CameraComp camera_;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    // viewport dimensions
    int width_ = 0;
    int height_ = 0;
};