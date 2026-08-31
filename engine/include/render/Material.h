#pragma once

#include "device/GPUPipeline.h"   // Your custom Shader class
#include "Texture.h"  // Your custom Texture class

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <unordered_map>

using MaterialHandle = uint32_t;


enum class ShadingModel
{
    Plain, // no lighting needed
    Phong,
    BlinnPhong,
    PBR
};

class Material {
public:

    ShadingModel shadingModel = ShadingModel::BlinnPhong;

    // Phong/Blinn-Phong material properties
    glm::vec3 baseColour = glm::vec3(1.0f);
    glm::vec3 specularColour = glm::vec3(1.0f);
    float shininess = 32.0f;

    // PBR models
    float metallic = 0.0f;
    float roughness = 1.0f;

    // Key = Uniform name in shader (e.g., "material.diffuseMap")
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    std::shared_ptr<GPUPipeline> gpuPipe;

public:
    Material(std::shared_ptr<GPUPipeline> shaderProgram);
    ~Material() = default;

    // Bind textures to appropriate GPU texture units and send uniforms to shader
    void bind() const;

    // Setters for textures and properties
    void setTexture(const std::string& uniformName, std::shared_ptr<Texture> texture);

    //void setBaseColor(const glm::vec3& c) { baseColour = c; }
    //void setSpecularColor(const glm::vec3& c) { specularColour = c; }
    //void setShininess(float value) { shininess = value; }

    // Getters
    // std::shared_ptr<GPUPipeline> getGPUPipeline() const { return gpuPipe; }
    std::shared_ptr<Texture> getTexture(const std::string& uniformName) const;
};