#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Shader.hpp"   // Your custom Shader class
#include "Texture.hpp"  // Your custom Texture class

enum class ShadingModel
{
    Phong,
    PBR
};

class Material {
public:

    ShadingModel shadingModel = ShadingModel::Phong;
    
    // Key = Uniform name in shader (e.g., "material.diffuseMap")
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    
    // Phong/Blinn-Phong material properties
    glm::vec3 baseColor = glm::vec3(1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f);
    float shininess = 32.0f;

    // PBR models
    float metallic = 0.0f;
    float roughness = 1.0f;

    std::shared_ptr<GPUPipeline> gpuPipe;

public:
    Material(std::shared_ptr<Shader> shaderProgram);
    ~Material() = default;

    // Bind textures to appropriate GPU texture units and send uniforms to shader
    void bind() const;

    // Setters for textures and properties
    void setTexture(const std::string& uniformName, std::shared_ptr<Texture> texture);
    void setDiffuseColor(const glm::vec3& color) { diffuseColor = color; }
    void setSpecularColor(const glm::vec3& color) { specularColor = color; }
    void setShininess(float value) { shininess = value; }

    // Getters
    std::shared_ptr<GPUPipeline> getGPUPipeline() const { return gpuPipe; }
    std::shared_ptr<Texture> getTexture(const std::string& uniformName) const;
};