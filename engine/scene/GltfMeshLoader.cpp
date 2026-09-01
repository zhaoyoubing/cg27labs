// engine/scene/GltfMeshLoader.cpp


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "scene/GltfMeshLoader.h"
#include "scene/Vertex.h"
//#include "scene/MeshGeoGPU.h"

#include "render/Texture.h"

#include <iostream>
#include <memory>
#include <filesystem>

std::shared_ptr<Mesh> GltfMeshLoader::loadModel(const std::string& filepath, TextureManager& textureManager) {
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
        std::cout << "[GltfLoader Warning]: " << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << "[GltfLoader Error]: " << err << std::endl;
    }
    if (!success) {
        std::cerr << "[GltfLoader] Failed to load parsed glTF: " << filepath << std::endl;
        return nullptr;
    }

    auto meshModel = std::make_shared<Mesh>();

    // 1. Load Textures from glTF
    std::vector<std::shared_ptr<Texture>> loadedTextures;
    for (size_t i = 0; i < gltfModel.images.size(); ++i) {
        const auto& img = gltfModel.images[i];

        std::string key;

        if (!img.uri.empty())
        {
            std::filesystem::path modelPath(filepath);
            std::filesystem::path texturePath = modelPath.parent_path() / img.uri;

            key = texturePath.lexically_normal().string();
        }
        else
        {
            key = filepath + "_image_" + std::to_string(i);
        }

        auto texture = textureManager.loadFromMemory(
            key,
            img.image.data(),
            img.width,
            img.height,
            img.component
        );

        loadedTextures.push_back(texture);
    }

    // 2. Helper lambda to extract vertex attributes safely from glTF accessors
    // (Assumes standard Vertex structure layout: Position(vec3), Normal(vec3), TexCoords(vec2))
    
    // Iterate over all nodes in the scene graph
    const tinygltf::Scene& scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
    for (size_t nodeIdx : scene.nodes) {
        const tinygltf::Node& node = gltfModel.nodes[nodeIdx];
        if (node.mesh < 0) continue;

        const tinygltf::Mesh& mesh = gltfModel.meshes[node.mesh];

        for (const auto& primitive : mesh.primitives) {
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
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                const auto& buffer = gltfModel.buffers[bufferView.buffer];
                const float* uvBuf = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

                for (size_t i = 0; i < accessor.count; ++i) {
                    vertices[i].uv = glm::vec2(uvBuf[i * 2 ], uvBuf[i * 2 + 1]);
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
            //auto geometry = std::make_shared<MeshGeometry>(vertices, indices);
            //geometry->setupMesh(vertices, indices);

            // 4. Track material index mapping
            unsigned int materialIdx = primitive.material >= 0 ? primitive.material : 0;

            // 5. Push sub-mesh into the container model
            //meshModel->addSubMesh(geometry, materialIdx);
        }
    }

    return meshModel;
}