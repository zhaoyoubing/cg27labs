// include/render/TextureManager.hpp
#pragma once
#include "render/Texture.h"

#include <spdlog/spdlog.h>

#include <string>
#include <memory>
#include <unordered_map>


class TextureManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

public:
    std::shared_ptr<Texture> loadTextureFromFile(const std::string& key, const std::string& filename);
    /*  {

        auto it = textures.find(key);

        if (it != textures.end())
        {
            return it->second;
        }


        // --------------------------------------------------
        // 2. Load image from disk
        // --------------------------------------------------

        int width = 0;
        int height = 0;
        int channels = 0;

        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            spdlog::error("Failed to load texture: {}", filename);
            return nullptr;
        }


        // --------------------------------------------------
        // 3. Determine OpenGL format
        // --------------------------------------------------

        GLenum format = GL_RGB;

        switch (channels)
        {
            case 1: format = GL_RED; break;
            case 2: format = GL_RG; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
            default:
                spdlog::error("Unsupported number of channels ({} in texture: {}", channels, filename);
                stbi_image_free(data);
                return nullptr;
        }

        // --------------------------------------------------
        // 4. Create Texture object
        // --------------------------------------------------
        auto texture =  std::make_shared<Texture>();

        texture->width = width;
        texture->height = height;
        texture->channels = channels;

        // --------------------------------------------------
        // 5. Create OpenGL texture
        // --------------------------------------------------

        glGenTextures(1, &texture->id);

        glBindTexture(GL_TEXTURE_2D, texture->id);

        // --------------------------------------------------
        // 6. Set texture parameters
        // --------------------------------------------------
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // --------------------------------------------------
        // 7. Upload image to GPU
        // --------------------------------------------------

        glTexImage2D(
            GL_TEXTURE_2D, 0, format,
            width, height, 0,
            format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        // --------------------------------------------------
        // 8. Free CPU image data
        // --------------------------------------------------
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);

        // --------------------------------------------------
        // 9. Store in manager
        // --------------------------------------------------
        textures.emplace(key, texture);

        return texture;

    }
*/
    // Load or retrieve an existing texture from raw memory (ideal for glTF embedded data)
    std::shared_ptr<Texture> loadFromMemory(const std::string& cacheKey, const unsigned char* data, int w, int h, int channels) {
        auto it = textures.find(cacheKey);
        if (it != textures.end()) {
            return it->second; // Return cached texture
        }

        auto texture = std::make_shared<Texture>(data, w, h, channels);
        textures[cacheKey] = texture;
        return texture;
    }

    void clear() {
        textures.clear();
    }
};