// include/render/TextureManager.hpp
#pragma once
#include "render/Texture.h"

#include <spdlog/spdlog.h>

#include <string>
#include <memory>
#include <unordered_map>

using TextureHandle = uint32_t;

class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
    std::shared_ptr<Texture> loadTextureFromFile(const std::string& key, const std::string& filename);

    // Load or retrieve an existing texture from raw memory (ideal for glTF embedded data)
    std::shared_ptr<Texture> loadFromMemory(const std::string& cacheKey, const unsigned char* data, int w, int h, int channels);

    void clear() {
        textures.clear();
    }
};