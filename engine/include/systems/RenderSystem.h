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
};