// engine/include/renderpasses/VolumetricFogPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"
#include "entity/LightComp.h"

// Screen space reflection pass
class VolumetricFogPass : public RenderPass
{
public:
    explicit VolumetricFogPass() { }
    
    // called once before the rendering loop
    bool init(RenderContext& context) override;

    // Render function called once per frame.
    void execute(RenderContext& context) override;

private:

    CameraComp camera_;
    LightComp lightDefault_;
    
    // framebuffer dimensions
    int width_ = 0;
    int height_ = 0;

    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    GLuint FBO = 0;
    GLuint texFBO = 0;
    
};