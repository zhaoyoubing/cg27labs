// include/ecs/components/MeshComp.h
#pragma once

#include <memory>
#include "scene/geometry/MeshGeometry.h"
#include "render/Material.hpp" // for textures, shaders, colors

// the renderable mesh
struct MeshComp {
    std::shared_ptr<MeshGeometry> geometry;
    std::shared_ptr<Material> material; // Unique material or shared
}