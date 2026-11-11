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
class BloomBlendPass : public RenderPass
{
public:
    explicit BloomBlendPass() { }
    
    bool init(RenderContext& context) override;

    // Called once per frame.
    void execute(RenderContext& context) override;

private:
    CameraComp camera_;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    // viewport dimensions
    int width_ = 0;
    int height_ = 0;

    GLuint FBO = 0;  // frame buffer object

};