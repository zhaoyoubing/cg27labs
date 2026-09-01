#pragma once

#include "Mesh.h"
#include "resources/TextureManager.h"
#include "resources/MaterialManager.h"

#include <string>
#include <memory>


class GltfMeshLoader {
public:
    // Loads a .gltf or .glb file and returns a fully initialized MeshModel
    static std::shared_ptr<Mesh> loadModel(const std::string& filepath, TextureManager& texMgr, MaterialManager & matMgr);
};