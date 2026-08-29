#pragma once

#include <memory>
#include "scene/MeshGeoGPU.h"
#include "render/Material.h"

struct MeshRenderable {
    std::shared_ptr<MeshGeoGPU> geometry;
    std::shared_ptr<Material> material;

    void draw() const {
        if (material) {
            material->bind(); // Binds shader & textures
        }
        geometry->draw(); // Binds VAO & draws triangles
    }
};