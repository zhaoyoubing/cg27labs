// engine/include/renderpasses/ForwardPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"

// SSAOBoxBlurPass Pass for AO map blurring
class SSAOBoxBlurPass : public RenderPass
{
public:
    explicit SSAOBoxBlurPass() { }

    // init function callded once before the render loop
    bool init(RenderContext& context) override;

    // render function called once per frame.
    void execute(RenderContext& context) override;

private:
    CameraComp camera_;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    int width_ = 0;  // framebuffer dimensions
    int height_ = 0;

    GLuint FBO = 0;  // framebuffer object
    GLuint texFBO = 0;  // framebuffer texture  
};