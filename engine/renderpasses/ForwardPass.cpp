#include "renderpasses/ForwardPass.h"

void ForwardPass::execute(RenderContext& context)
{
    auto view = context.registry.view<
        TransformComponent,
        MeshComponent,
        MaterialComponent>();

    for (auto entity : view)
    {
        // render entity
    }
}