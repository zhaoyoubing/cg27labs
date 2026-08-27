// include/render/TextureManager.hpp
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "render/Texture.h"

class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;

public:
    // Load or retrieve an existing texture from raw memory (ideal for glTF embedded data)
    std::shared_ptr<Texture> loadFromMemory(const std::string& cacheKey, const unsigned char* data, int w, int h, int channels) {
        auto it = textureCache.find(cacheKey);
        if (it != textureCache.end()) {
            return it->second; // Return cached texture
        }

        auto texture = std::make_shared<Texture>(data, w, h, channels);
        textureCache[cacheKey] = texture;
        return texture;
    }

    void clear() {
        textureCache.clear();
    }
};