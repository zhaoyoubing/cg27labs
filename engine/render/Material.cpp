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
    //gpuPipe->setVec3("material.baseColor", baseColour);
    //gpuPipe->setVec3("material.specularColor", specularColour);
    //gpuPipe->setFloat("material.shininess", shininess);


    for (const auto& [name, value] : parameters)
    {
        std::visit(
            [&](const auto& val)
            {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, float>) {
                    gpuPipe->setFloat(name, val); // Explicitly compiles for float
                }
                else if constexpr (std::is_same_v<T, int>) {
                    gpuPipe->setInt(name, val); // Explicitly compiles for int
                } else if constexpr (std::is_same_v<T, glm::vec2>) {
                    gpuPipe->setVec2(name, val);
                } else if constexpr (std::is_same_v<T, glm::vec3>) {
                    gpuPipe->setVec3(name, val);
                } else if constexpr (std::is_same_v<T, glm::vec4>) {
                    gpuPipe->setVec4(name, val);
                } else if constexpr (std::is_same_v<T, glm::mat3>) {
                    gpuPipe->setMat4(name, val);
                } else if constexpr (std::is_same_v<T, glm::mat4>) {
                    gpuPipe->setMat4(name, val);
                }
            },
            value);
    }


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