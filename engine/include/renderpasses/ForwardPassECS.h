#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"

#include <memory>


class ForwardPassECS : public RenderPass
{
public:
    explicit ForwardPassECS() { }
    explicit ForwardPassECS(std::shared_ptr<GPUPipeline> gpuPipe) { }

    // Called once per frame.
    void execute(RenderContext& context) override;

//private:
    //Framebuffer framebuffer_;
};