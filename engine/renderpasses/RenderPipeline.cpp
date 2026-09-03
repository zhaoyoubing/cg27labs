#include "renderpasses/RenderPipeline.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderContext.h"

void RenderPipeline::addPass(std::unique_ptr<RenderPass> pass)
{
    passes_.push_back(std::move(pass));
}

void RenderPipeline::render(RenderContext& context)
{
    for (auto& pass : passes_)
    {
        pass->execute(context);
    }
}