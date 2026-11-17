// engine/include/renderpasses/GaussianBlurPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"
#include "entity/LightComp.h"

// Gaussian Blur Pass for post-processing
class GaussianBlurPass : public RenderPass
{
public:
    explicit GaussianBlurPass() { }
    
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

    GLuint FBO = 0;  // frame buffer object

    // buffer A and B for ping-pong Gaussian blurring
    GLuint texBufA = 0;  
    GLuint texBufB = 0;  
};