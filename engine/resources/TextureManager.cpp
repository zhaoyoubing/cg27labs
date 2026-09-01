#include "resources/TextureManager.h"

#include <stb_image.h>
#include <spdlog/spdlog.h>


std::shared_ptr<Texture> TextureManager::loadTextureFromFile(const std::string& key, const std::string& filename) 
{
        auto it = textures.find(key);

        if (it != textures.end()) {
            return it->second;
        }

        int width = 0;
        int height = 0;
        int channels = 0;


        // Load image from disk 
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

        if (!data) {
            spdlog::error("Failed to load texture: {}", filename);
            return nullptr;
        }

        // Determine OpenGL format
        TextureFormat format = TextureFormat::RGB8;

        switch (channels) {
            case 1: format = TextureFormat::RGB8; break;
            case 2: format = TextureFormat::RG8; break;
            case 3: format = TextureFormat::RGB32F; break;
            case 4: format = TextureFormat::RGBA32F; break;
            default:
                spdlog::error("Unsupported number of channels ({}) in texture {}", channels, filename);
                stbi_image_free(data);
                return nullptr;
        }



        // Create Texture object
        auto texture = std::make_shared<Texture>(width, height, format, data);

        stbi_image_free(data);

        textures.emplace(key, texture);

        return texture;
}


std::shared_ptr<Texture> TextureManager::loadFromMemory(const std::string& cacheKey, const unsigned char* data, int w, int h, int channels) {
    auto it = textures.find(cacheKey);
    if (it != textures.end()) {
        return it->second; // Return cached texture
    }

    // Determine OpenGL format
    TextureFormat format = TextureFormat::RGB8;

    switch (channels) {
        case 1: format = TextureFormat::RGB8; break;
        case 2: format = TextureFormat::RG8; break;
        case 3: format = TextureFormat::RGB32F; break;
        case 4: format = TextureFormat::RGBA32F; break;
        default:
            spdlog::error("Unsupported number of channels ({}) in texture {}", channels, cacheKey);
            //(data);
            return nullptr;
    }


    auto texture = std::make_shared<Texture>(w, h, format, data);
    textures[cacheKey] = texture;
    return texture;
}
