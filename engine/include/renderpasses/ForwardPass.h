#pragma once

#include <memory>
#include "device/GPUPipeline.h"
class ForwardPass : public RenderPass
{
public:
    explicit ForwardPass(std::shared_ptr<GPUPipeline> gpuPipe) { }
private:
    //Framebuffer framebuffer_;
};