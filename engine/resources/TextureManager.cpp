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

        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

        if (!data) {
            spdlog::error("Failed to load texture: {}", filename);
            return nullptr;
        }

        GLenum format = GL_RGB;

        switch (channels) {
            case 1: format = GL_RED; break;
            case 2: format = GL_RG; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default:
                spdlog::error("Unsupported number of channels ({}) in texture: {}", channels, filename);
                stbi_image_free(data);
                return nullptr;
        }

        auto texture = std::make_shared<Texture>(data, width, height, channels);

        stbi_image_free(data);

        textures.emplace(key, texture);

        return texture;
    }