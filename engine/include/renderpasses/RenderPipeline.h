// engine/include/renderpasses/RenderPipeline.h
#pragma once

#include "RenderPass.h"

#include <memory>
#include <vector>

class RenderPipeline
{
public:

    void addPass(std::unique_ptr<RenderPass> pass);
    void init(RenderContext& context);
    void render(RenderContext& context);
    // for future support of window resizing
    // void onResize(int w, int h);
    
private:
    std::vector<std::unique_ptr<RenderPass>> passes_;
};