#pragma once

#include <memory>
#include "model/MeshGeometry.h"
// #include "render/Material.hpp" // Optional: for textures, shaders, colors

struct MeshComp {
    std::shared_ptr<MeshGeometry> geometry;
    std::shared_ptr<Material> material; // Unique material or shared
}