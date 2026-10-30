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

void Material::bind(const std::shared_ptr<GPUPipeline> pipe) {

    if (nullptr != pipe)
        gpuPipe = pipe;

    if (!gpuPipe) {
        spdlog::error("gpuPipe null in Material::bind()");
        return;
    }

    // 1. Activate the shader program
    gpuPipe->bind();

    // temporarily
    // bind base colour and normal map texture
    if (nullptr != baseColourTex) {
        baseColourTex->bind(0);
        gpuPipe->setInt("baseColourMap", 0);
    }

    if (nullptr != normalTex) {
        normalTex->bind(1);
        gpuPipe->setInt("normalMap", 1);
    }
 
    // 2. Pass material property uniforms
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

void Material::unbind() {

    if (nullptr != baseColourTex) {
        baseColourTex->unbind(0);
    }

    if (nullptr != normalTex) {
        normalTex->unbind(1);
    }

    if (nullptr != gpuPipe) {
        gpuPipe->unbind();
    }
}