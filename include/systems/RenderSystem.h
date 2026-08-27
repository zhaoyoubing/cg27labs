#pragma once

#include "ecs/EcsWorldPool.h"
#include "render/Shader.h"

class RenderSystem {

public:
    void render(ECSWorldPool& ecsWorld, GPUPipeline & pipe) {
        auto view = ecsWorld.registry.view<TransformComponent, MeshComponent>();
        
        for (auto entity : view) {
            auto& [transform, meshComp] = view.get<TransformComponent, MeshComponent>(entity);

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
};