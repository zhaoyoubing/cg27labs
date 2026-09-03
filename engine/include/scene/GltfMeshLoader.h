#pragma once

#include "MeshGeometry.h"
#include "resources/TextureManager.h"
#include "resources/MaterialManager.h"
#include "resources/ShaderManager.h"

#include "scene/SceneNode.h"

#include "entity/TransformComp.h"


#include <string>
#include <memory>
#include <unordered_map>

namespace tinygltf
{
    class Model;
    class Node;
    class Mesh;
    //class Primitive;
}


class GltfMeshLoader {
public:
    // Loads a .gltf or .glb file and returns a fully initialized MeshModel
    static std::unique_ptr<SceneNode> loadModel(const std::string& filepath, 
        TextureManager& texMgr, MaterialManager & matMgr, ShaderManager & shaderMgr);

    static std::vector<std::shared_ptr<MaterialMesh> >  loadMesh(const tinygltf::Model & model, const tinygltf::Mesh & mesh,
            TextureManager& texMgr, MaterialManager & matMgr, ShaderManager & shaderMgr);

    static std::unique_ptr<SceneNode> loadNode(tinygltf::Model & model, tinygltf::Node & gltfNode,
            TextureManager& texMgr, MaterialManager & matMgr, ShaderManager & shaderMgr);

    static TransformComp loadTransform(tinygltf::Node & node);


    static std::unordered_map<int, std::shared_ptr<Material> > loadedMaterials;
    static std::vector<std::shared_ptr<Texture>> loadedTextures;

};