// engine/scene/GltfMeshLoader.cpp

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "scene/GltfMeshLoader.h"
#include "scene/Vertex.h"
#include "scene/SceneNode.h"

#include "render/Texture.h"

#include "device/MeshBufferGPU.h"

#include "glm/gtc/quaternion.hpp"

#include <spdlog/spdlog.h>

#include <iostream>
#include <memory>
#include <filesystem>

std::unordered_map<int, std::shared_ptr<Material> > GltfMeshLoader::loadedMaterials;
std::vector<std::shared_ptr<Texture>> GltfMeshLoader::loadedTextures;

TransformComp GltfMeshLoader::loadTransform(tinygltf::Node & gltfNode) {
    TransformComp trans {.bMatrixDirty = true};

    if (!gltfNode.matrix.empty())
    {
        glm::mat4 matrix(1.0f);

        for (int i = 0; i < 16; ++i)
        {
            matrix[i / 4][i % 4] =
                static_cast<float>(gltfNode.matrix[i]);
        }

        trans.mat = matrix;
        trans.type = TransformType::MATRIX;
    }
    else
    {
        if (gltfNode.translation.size() == 3 ) {
            trans.pos = glm::vec3(
                static_cast<float>(gltfNode.translation[0]),
                static_cast<float>(gltfNode.translation[1]),
                static_cast<float>(gltfNode.translation[2])
            );
        }

        if (gltfNode.rotation.size() == 4 ) {
            trans.quat = glm::quat(
                static_cast<float>(gltfNode.rotation[3]),
                static_cast<float>(gltfNode.rotation[0]),
                static_cast<float>(gltfNode.rotation[1]),
                static_cast<float>(gltfNode.rotation[2])
            );
            // convert quaternion to euler angles
            trans.rot = glm::eulerAngles(trans.quat);
        }

        if (gltfNode.scale.size() == 3 ) {
            trans.scale = glm::vec3(
                static_cast<float>(gltfNode.scale[0]),
                static_cast<float>(gltfNode.scale[1]),
                static_cast<float>(gltfNode.scale[2])
            );
        }

        trans.type = TransformType::QUATERNION;
    }

    return trans;
}

std::vector<std::shared_ptr<MaterialMesh> >  GltfMeshLoader::loadMesh(const tinygltf::Model & gltfModel, 
    const tinygltf::Mesh & mesh, TextureManager& texMgr, MaterialManager& matMgr, ShaderManager & shaderMgr) 
{
    std::vector<std::shared_ptr<MaterialMesh> > meshList;

    for (const auto& primitive : mesh.primitives) 
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // --- EXTRACT POSITIONS ---
        const float* posBuf = nullptr;
        if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
            const auto& accessor = gltfModel.accessors[primitive.attributes.find("POSITION")->second];
            const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
            const auto& buffer = gltfModel.buffers[bufferView.buffer];
            posBuf = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));
            
            vertices.resize(accessor.count);
            for (size_t i = 0; i < accessor.count; i++) {
                vertices[i].pos = glm::vec3( posBuf[i * 3 ], posBuf[i * 3 + 1], posBuf[i * 3 + 2] );
            }
        }

        // --- EXTRACT NORMALS ---
        if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
            const auto& accessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->second];
            const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
            const auto& buffer = gltfModel.buffers[bufferView.buffer];
            const float* normalBuf = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

            for (size_t i = 0; i < accessor.count; i++) {
                vertices[i].normal = glm::vec3( normalBuf[i * 3], normalBuf[i * 3 + 1], normalBuf[i * 3 + 2] );
            }
        }

        // --- EXTRACT UVs (TEXCOORD_0) ---
        if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
            const auto& accessor = gltfModel.accessors[primitive.attributes.find("TEXCOORD_0")->second];

            spdlog::debug(
                "UV count = {}, componentType = {}, type = {}, bufferView = {}, "
                "byteOffset = {}, normalized = {}",
                accessor.count,
                accessor.componentType,
                accessor.type,
                accessor.bufferView,
                accessor.byteOffset,
                accessor.normalized
            );

            const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];

            spdlog::debug(
                "componentType = {}, type = {}, count = {}, stride = {}, bufferView offset = {}",
                accessor.componentType,
                accessor.type,
                accessor.count,
                accessor.ByteStride(bufferView),
                bufferView.byteOffset
            );

            spdlog::debug(
                "bufferView.byteOffse = {}, accessor.byteOffset = {}, bufferView.byteStride = {}, accessor.count = {}",
                bufferView.byteOffset,
                accessor.byteOffset,
                bufferView.byteStride,
                accessor.count
            );

            size_t stride = accessor.ByteStride(bufferView);
            spdlog::debug("Texture buffer stride {}", stride);

            if (stride == 0)
                stride = sizeof(float) * 2;

            const auto& buffer = gltfModel.buffers[bufferView.buffer];
            //const float* uvBuf = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

            const unsigned char* data = buffer.data.data()
                        + bufferView.byteOffset + accessor.byteOffset;


            for (size_t i = 0; i < accessor.count; ++i) {
                const float* uv =  reinterpret_cast<const float*>(data + i * stride);
                vertices[i].uv = glm::vec2(uv[0], uv[1]);
                spdlog::trace("Texture u {}, v {}", uv[0], uv[1]);
            }
        }

        // --- EXTRACT INDICES ---
        // model not empty
        if (primitive.indices >= 0) {
            const auto& accessor = gltfModel.accessors[primitive.indices];
            const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
            const auto& buffer = gltfModel.buffers[bufferView.buffer];

            indices.resize(accessor.count);

            // glTF indices can be unsigned byte, unsigned short, or unsigned int
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                const uint16_t* buf = reinterpret_cast<const uint16_t*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));
                for (size_t i = 0; i < accessor.count; ++i) indices[i] = buf[i];
            } 
            else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                const uint32_t* buf = reinterpret_cast<const uint32_t*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
                for (size_t i = 0; i < accessor.count; ++i) indices[i] = buf[i];
            } 
            else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                const unsigned char* buf = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
                for (size_t i = 0; i < accessor.count; ++i) indices[i] = buf[i];
            }
        }

        // 3. Build low-level MeshGeometry
        auto geometry = std::make_shared<MeshGeometry>(vertices, indices);
        //geometry->setupMesh(vertices, indices);
        
        // add geometry
        auto materialMesh = std::make_shared<MaterialMesh>();
        materialMesh->geometry_ = geometry;

        // generate gpu buffers, have we considered sharing ?
        std::shared_ptr<MeshBufferGPU> meshBuf = std::make_shared<MeshBufferGPU>();
        meshBuf->createAndUploadBuffers(geometry);
        materialMesh->geometry_->gpuBuffer = meshBuf;

        // 4. Track material index mapping
        unsigned int materialIdx = primitive.material >= 0 ? primitive.material : 0;
        if (materialIdx >= 0)
        {
            if (loadedMaterials[materialIdx] == nullptr) {
                // materials can be shared
                const tinygltf::Material& gltfMaterial = gltfModel.materials[materialIdx];
                const auto& pbr = gltfMaterial.pbrMetallicRoughness;
                std::vector<double> baseColor = pbr.baseColorFactor;

                std::shared_ptr<GPUPipeline> gpuPipe = shaderMgr.get("texture_plain");
                std::shared_ptr<Material> material = std::make_shared<Material>(gpuPipe);

                material->baseColour = glm::vec4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]);
                material->roughness = pbr.roughnessFactor;
                material->metallic = pbr.metallicFactor;

                std::shared_ptr<Texture> baseColorTexture = nullptr;
                if (pbr.baseColorTexture.index >= 0) {
                        // Use base color texture
                        int textureIndex = pbr.baseColorTexture.index;
                        const tinygltf::Texture& texGltf = gltfModel.textures[textureIndex];

                        spdlog::debug("Set material {} texture {}", materialIdx, textureIndex);

                        material->baseColorTexture = loadedTextures[texGltf.source];

                }
                loadedMaterials[materialIdx] = material;
                matMgr.add(material);
            }

            materialMesh->material_ = loadedMaterials[materialIdx];
        }
        meshList.push_back(materialMesh);
    }    
        
    return meshList;
}

std::unique_ptr<SceneNode> GltfMeshLoader::loadNode(tinygltf::Model & gltfModel, tinygltf::Node & gltfNode,
    TextureManager& texMgr, MaterialManager& matMgr, ShaderManager & shaderMgr) 
{
        // recursively loading
        // tinygltf::Node& gltfNode = gltfModel.nodes[childIdx];

        auto node = std::make_unique<SceneNode>();

        // Load transform
        node->trans = loadTransform(gltfNode);

        // Mesh is optional
        if (gltfNode.mesh >= 0)
        {
            const tinygltf::Mesh& mesh = gltfModel.meshes[gltfNode.mesh];
            node->meshList = loadMesh(gltfModel, mesh,
                                    texMgr, matMgr, shaderMgr);
        }

        // Always process children
        for (int childIdx : gltfNode.children)
        {
            node->children.push_back(
                loadNode(gltfModel, gltfModel.nodes[childIdx], texMgr, matMgr, shaderMgr));
        }

        return node;
}


std::unique_ptr<SceneNode> GltfMeshLoader::loadModel(const std::string& filepath, 
    TextureManager& texMgr, MaterialManager& matMgr, ShaderManager & shaderMgr) 
{
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool success = false;
    // Check if it's binary (.glb) or text (.gltf)
    if (filepath.substr(filepath.find_last_of(".") + 1) == "glb") {
        success = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, filepath);
    } else {
        success = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, filepath);
    }

    if (!warn.empty()) {
        spdlog::warn("[GltfLoader Warning]: {}", warn);
    }
    if (!err.empty()) {
        spdlog::error("[GltfLoader Error]: {}", err);
    }
    if (!success) {
        spdlog::error("[GltfLoader] Failed to load parsed glTF: {}", filepath);
        return nullptr;
    }

    loadedMaterials.clear();
    loadedTextures.clear();

    // 1. Load Textures from glTF
    
    for (size_t i = 0; i < gltfModel.images.size(); ++i) {
        const auto& img = gltfModel.images[i];

        std::string key;

        if (!img.uri.empty()) {
            //std::filesystem::path modelPath(filepath);
            //std::filesystem::path texturePath = modelPath.parent_path() / img.uri;
            //key = texturePath.lexically_normal().string();
            key = filepath + "_image_" + std::to_string(i) + "_" + img.uri;
        } else if (!img.name.empty()) {
            key = filepath + "_image_" + std::to_string(i);
        }

       spdlog::debug("Loading glTF texture image {}: {}, width = {}, height = {}, components = {}, bits = {}", 
                i, img.name, img.width, img.height, img.component, img.bits);

        auto texture = texMgr.loadFromMemory(
            key, img.image.data(), img.width, img.height, img.component
        );

        loadedTextures.push_back(texture);
    }

    // 2. Helper lambda to extract vertex attributes safely from glTF accessors
    // (Assumes standard Vertex structure layout: Position(vec3), Normal(vec3), TexCoords(vec2))
    
    if (gltfModel.scenes.empty())
    {
        // handle invalid/scene-less glTF
        spdlog::warn("glTF model empty: {}", filepath);
        return nullptr;
    }

    std::unique_ptr<SceneNode> meshRoot = std::make_unique<SceneNode>();

    // only loading the default scene
    const tinygltf::Scene& scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
    // Iterate over all nodes in the scene graph
    for (size_t nodeIdx : scene.nodes) {
        tinygltf::Node& node = gltfModel.nodes[nodeIdx];

        std::shared_ptr<SceneNode> meshNode = loadNode(gltfModel, node, texMgr, matMgr, shaderMgr);

        //const tinygltf::Mesh& mesh = gltfModel.meshes[node.mesh];
        // 5. Push sub-mesh into the container model
        meshRoot->children.push_back(meshNode->clone());
    }

    return meshRoot;
}