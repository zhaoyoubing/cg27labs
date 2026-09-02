#pragma once

#include "MeshGeometry.h"
#include "resources/TextureManager.h"
#include "resources/MaterialManager.h"

#include "scene/SceneNode.h"

#include <string>
#include <memory>
#include <unordered_map>


class GltfMeshLoader {
public:
    // Loads a .gltf or .glb file and returns a fully initialized MeshModel
    static std::unique_ptr<SceneNode> loadModel(const std::string& filepath, TextureManager& texMgr, MaterialManager & matMgr);

};