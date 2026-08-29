#pragma once

#include <string>
#include <memory>
#include "scene/MeshModel.h"
#include "render/TextureManager.h"

class GltfMeshLoader {
public:
    // Loads a .gltf or .glb file and returns a fully initialized MeshModel
    static std::shared_ptr<MeshModel> loadModel(const std::string& filepath, TextureManager& texMgr);
};