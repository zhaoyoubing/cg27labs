#include "renderpasses/ForwardPass.h"

void ForwardPass::execute(RenderContext& context)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(
        0,
        0,
        context.viewportWidth,
        context.viewportHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = context.registry.view<
        TransformComponent,
        MeshComponent,
        MaterialComponent>();

    for (auto entity : view)
    {
              const auto& transform =
            view.get<TransformComponent>(entity);

        const auto& mesh =
            view.get<MeshComponent>(entity);

        const auto& materialComponent =
            view.get<MaterialComponent>(entity);

        if (!mesh.mesh)
            continue;

        Material& material =
            context.resources.materials().get(
                materialComponent.material);

        // Select the material's shader
        material.shader->bind();

        material.shader->setMat4(
            "uModel",
            transform.matrix);

        material.shader->setMat4(
            "uView",
            context.camera.view);

        material.shader->setMat4(
            "uProjection",
            context.camera.projection);

        material.shader->setVec4(
            "uBaseColour",
            material.baseColour);

        mesh.mesh->bindAndDraw();

        material.shader->unbind();
    }
}