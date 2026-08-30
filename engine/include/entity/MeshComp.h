// include/entity/MeshComp.h
#pragma once

#include <memory>
#include "scene/MeshBufferGPU.h"
#include "render/Material.h" // for textures, shaders, colors

// the renderable mesh
struct MeshComp {
    std::shared_ptr<MeshBufferGPU> meshBuf;
    std::shared_ptr<Material> material; // Unique material or shared
};