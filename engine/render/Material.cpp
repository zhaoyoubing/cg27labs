#include <render/Material.h>

Material::Material(std::shared_ptr<GPUPipeline> pipe) : gpuPipe(pipe) {}

void Material::setTexture(const std::string& uniformName, std::shared_ptr<Texture> texture) {
    textures[uniformName] = texture;
}

std::shared_ptr<Texture> Material::getTexture(const std::string& uniformName) const {
    auto it = textures.find(uniformName);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

void Material::bind() const {
    if (!gpuPipe) return;

    // 1. Activate the shader program
    gpuPipe->bind();

    // 2. Pass material color/property uniforms
    gpuPipe->setVec3("material.baseColor", baseColour);
    gpuPipe->setVec3("material.specularColor", specularColour);
    gpuPipe->setFloat("material.shininess", shininess);

    // 3. Bind all associated textures dynamically
    unsigned int textureUnit = 0;
    for (const auto& [uniformName, texture] : textures) {
        if (texture) {
            texture->bind(textureUnit);            // Active texture unit (glActiveTexture)
            gpuPipe->setInt(uniformName, textureUnit); // Tell shader which sampler corresponds to this unit
            textureUnit++;
        }
    }
}