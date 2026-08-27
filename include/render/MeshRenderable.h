#pragma once

#include <memory>
#include "MeshGPU.h"
#include "Material.h"

struct MeshRenderable {
    std::shared_ptr<MeshGPU> geometry;
    std::shared_ptr<Material> material;

    void draw() const {
        if (material) {
            material->bind(); // Binds shader & textures
        }
        geometry->draw(); // Binds VAO & draws triangles
    }
};