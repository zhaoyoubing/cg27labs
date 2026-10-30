#include "renderpasses/RenderPipeline.h"
#include "renderpasses/RenderPass.h"
#include "renderpasses/RenderContext.h"

void RenderPipeline::addPass(std::unique_ptr<RenderPass> pass)
{
    passes_.push_back(std::move(pass));
}

void RenderPipeline::render(RenderContext& context)
{
    int i = 0;
    for (auto& pass : passes_)
    {
        spdlog::trace("execute pass {}", i++); 
        pass->execute(context);
        spdlog::trace("shadow map id {}", context.shadow_.depthMap);
    }
}

void RenderPipeline::init(RenderContext& context)
{
    spdlog::debug("Init RenderPipeline");
    
    for (auto& pass : passes_)
    {
        pass->init(context);
    }
}
