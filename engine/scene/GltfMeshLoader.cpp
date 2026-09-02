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

#include <iostream>
#include <memory>
#include <filesystem>

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

    std::unordered_map<int, std::shared_ptr<Material> > loadedMaterials;
    auto meshRoot = std::make_unique<SceneNode>();

    // 1. Load Textures from glTF
    std::vector<std::shared_ptr<Texture>> loadedTextures;
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

        auto texture = texMgr.loadFromMemory(
            key, img.image.data(), img.width, img.height, img.component
        );

        loadedTextures.push_back(texture);
    }

    // 2. Helper lambda to extract vertex attributes safely from glTF accessors
    // (Assumes standard Vertex structure layout: Position(vec3), Normal(vec3), TexCoords(vec2))
    
    // Iterate over all nodes in the scene graph
    const tinygltf::Scene& scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];
    for (size_t nodeIdx : scene.nodes) {
        const tinygltf::Node& node = gltfModel.nodes[nodeIdx];
        /*
        // TODO recursively loading
        
        auto node = std::make_unique<Node>();

        // Load transform
        node->transform = loadTransform(gltfNode);

        // Mesh is optional
        if (gltfNode.mesh >= 0)
        {
            node->renderable = loadMesh(
                gltfModel,
                gltfNode.mesh);
        }

        // Always process children
        for (int childIdx : gltfNode.children)
        {
            node->children.push_back(
                loadNode(gltfModel, childIdx));
        }

        return node;

        */
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
            auto geometry = std::make_shared<MeshGeometry>(vertices, indices);
            //geometry->setupMesh(vertices, indices);
            
            // add geometry
            auto meshRenderable = std::make_shared<MaterialMesh>();
            meshRenderable->geometry_ = geometry;

            // generate gpu buffers, have we considered sharing ?
            std::shared_ptr<MeshBufferGPU> meshBuf = std::make_shared<MeshBufferGPU>();
            meshBuf->createAndUploadBuffers(geometry);
            meshRenderable->geometry_->gpuBuffer = meshBuf;

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

                            material->baseColorTexture = loadedTextures[texGltf.source];

                            //material.setTexture("baseColorTexture", texture);

                    }
                    loadedMaterials[materialIdx] = material;
                    meshRenderable->material_ = loadedMaterials[materialIdx];
                    matMgr.add(material);
                }

            }

            auto meshNode = std::make_shared<SceneNode>();
            meshNode->renderable = meshRenderable;

            // 5. Push sub-mesh into the container model
            meshRoot->children.push_back(meshNode->clone());
        }
    }

    return meshRoot;
}