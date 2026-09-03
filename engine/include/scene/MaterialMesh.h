#pragma once

#include "MeshGeometry.h"
#include "render/Material.h"

#include <glm/glm.hpp>
#include <memory>

struct MaterialMesh {
public:
    std::shared_ptr<MeshGeometry> geometry_;
    std::shared_ptr<Material> material_;

    MaterialMesh() = default;
    MaterialMesh(std::shared_ptr<MeshGeometry> mesh) : geometry_(mesh) {}

};
