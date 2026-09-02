#pragma once

#include "device/GPUPipeline.h"   // Your custom Shader class
#include "Texture.h"  // Your custom Texture class

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <variant>
#include <unordered_map>


using MaterialHandle = uint32_t;


enum class ShadingModel
{
    Plain, // base colour, no lighting needed
    Phong,
    BlinnPhong,
    PBR
};

using MaterialValue = std::variant<
    int,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4
>;


class Material {
public:
    std::string name;
    
    ShadingModel shadingModel = ShadingModel::BlinnPhong;

    // Phong/Blinn-Phong material properties
    glm::vec4 baseColour = glm::vec4(1.0f);

    glm::vec4 specularColour = glm::vec4(1.0f);
    float shininess = 32.0f;

    // PBR models
    float metallic = 0.0f;
    float roughness = 1.0f;

    std::shared_ptr<Texture> baseColorTexture;
    std::shared_ptr<Texture> normalTexture;
    // std::shared_ptr<Texture> metallicRoughnessTexture;
   
    // additional textures, Key = Uniform name in shader (e.g., "material.metalRoughMap")
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    // general parameters, Key = Uniform name in shader (e.g., "material.roughness")
    std::unordered_map<std::string, MaterialValue> parameters;

    std::shared_ptr<GPUPipeline> gpuPipe;

public:
    Material() = default;
    Material(std::shared_ptr<GPUPipeline> shaderProgram);
    ~Material() = default;

    // Bind textures to appropriate GPU texture units and send uniforms to shader
    void bind() const;

    // Setters for textures and properties
    void setTexture(const std::string& uniformName, std::shared_ptr<Texture> texture);

    // Getters
    // std::shared_ptr<GPUPipeline> getGPUPipeline() const { return gpuPipe; }
    std::shared_ptr<Texture> getTexture(const std::string& uniformName) const;
};