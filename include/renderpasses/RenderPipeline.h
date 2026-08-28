#pragma once

#include <memory>
#include <vector>

class RenderPipeline
{
public:
    void addPass(std::unique_ptr<RenderPass> pass);
    void render(RenderContext& context);

private:
    std::vector<std::unique_ptr<RenderPass>> passes_;
};