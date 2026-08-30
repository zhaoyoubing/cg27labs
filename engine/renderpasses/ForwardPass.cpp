#include "renderpasses/ForwardPass.h"

#include "entity/TransformComp.h"
#include "entity/MaterialComp.h"
#include "entity/MeshComp.h"

void ForwardPass::execute(RenderContext& context)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(
        0, 0, context.camera_.viewport.w, context.camera_.viewport.h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = context.registry_.view<
        TransformComp,
        MeshComp,
        MaterialComp>();

    for (auto entity : view)
    {
        const auto& transform =
            context.registry_.getComp<TransformComp>(entity);

        const auto& mesh =
            context.registry_.getComp<MeshComp>(entity);

        const auto& materialComp =
           context.registry_.getComp<MaterialComp>(entity);

        if (! mesh.meshBuf)
            continue;

        Material& material =
            context.matMgr_.get(materialComp.material);

        // Select the material's shader
        material.gpuPipe->bind();

        material.gpuPipe->setMat4("matModel", transform.getLocalMatrix());
        material.gpuPipe->setMat4("matView",  context.camera_.getViewMatrix());
        material.gpuPipe->setMat4( "matProj", context.camera_.getProjMatrix());
        material.gpuPipe->setVec3( "uBaseColour",  material.baseColour);

        mesh.meshBuf->bind();
        mesh.meshBuf->draw();
        mesh.meshBuf->unbind();

        material.gpuPipe->unbind();
    }
}