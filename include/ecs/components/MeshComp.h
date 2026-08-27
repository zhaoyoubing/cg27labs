// include/ecs/components/MeshComp.h
#pragma once

#include <memory>
#include "scene/MeshGeometry.h"
// #include "render/Material.hpp" // [TODO]: for textures, shaders, colors

struct MeshComp {
    std::shared_ptr<MeshGeometry> geometry;
    std::shared_ptr<Material> material; // Unique material or shared
}