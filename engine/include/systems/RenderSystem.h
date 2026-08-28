#pragma once

#include "entity/EcsWorldRegistry.h"
#include "render/Shader.h"

class RenderSystem {

public:
    void render(ECSWorldRegistry & world, GPUPipeline & pipe) {
        auto view = world.view<TransformComponent, MeshComponent>();
        
        for (auto entity : view) {
            auto& [transform, meshComp] = view.get<TransformComp, MeshComp>(entity);

            if (!meshComp.geometry) continue;

            // 1. Set up matrices
            glm::mat4 modelMatrix = transform.getTransformMatrix();
            //defaultShader.setUniform("uModel", modelMatrix);

            // 2. Bind material/textures if available
            // ...

            // 3. Draw the shared geometry
            //meshComp.geometry->bindAndDraw();
        }
    }

    // 1. Simple mode: Zero passes exposed to the user, draws straight to screen
    void DrawDirect(entt::registry& registry, Shader& shader) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_WindowWidth, m_WindowHeight);
        shader.Bind();

        auto view = registry.view<TransformComponent, MeshRendererComponent>();
        view.each([&shader](auto& transform, auto& meshRenderer) {
            shader.SetMat4("u_ModelMatrix", transform.GetMatrix());
            meshRenderer.vertexArray->Bind();
            glDrawElements(GL_TRIANGLES, meshRenderer.indexCount, GL_UNSIGNED_INT, 0);
        });
    }

    // 2. Advanced mode: Full multi-pass orchestration driven by the ECS
    void ExecutePass(entt::registry& registry, RenderPass& pass, PipelineState& pipeline, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
        pass.begin();
        pipeline.Bind();

        auto view = registry.view<TransformComponent, MeshRendererComponent>();
        view.each([&pipeline, &viewMatrix, &projectionMatrix](auto& transform, auto& meshRenderer) {
            // Optional: Skip meshes that don't match certain pass criteria if needed
            pipeline.shader->SetMat4("u_ModelMatrix", transform.GetMatrix());
            pipeline.shader->SetMat4("u_ViewMatrix", viewMatrix);
            pipeline.shader->SetMat4("u_ProjectionMatrix", projectionMatrix);

            meshRenderer.vertexArray->Bind();
            glDrawElements(GL_TRIANGLES, meshRenderer.indexCount, GL_UNSIGNED_INT, 0);
        });

        pass.end();
    }

private:
    int m_WindowWidth = 1280;
    int m_WindowHeight = 720;
};