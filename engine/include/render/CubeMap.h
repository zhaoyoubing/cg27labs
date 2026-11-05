#pragma once

#include <glad/glad.h>
#include <string>

//#include "EnvironMap.h"
#include "stb_image.h"
#include "spdlog/spdlog.h"

class CubeMap {
public:
    GLuint mapId_ = 0; // OpenGL texture ID
    int width_ = 0;
    int height_ = 0;
    GLenum format_ = GL_RGBA; // Default format

    virtual ~CubeMap() = default;

    void bind(GLuint unit) const {  
        //glActiveTexture(GL_TEXTURE0 + unit);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, mapId_);

        spdlog::trace("Binding CubeMap texture ID {} to unit {}", mapId_, unit);
        glBindTextureUnit(unit, mapId_);
    }

    bool load(const std::vector<std::string>& faceFiles) {
        // Load the cube map texture from file (implementation not shown)
        // Set mapId_, width_, height_, and format_ accordingly

        // OpenGL 4.5+ Direct State Access (DSA)
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mapId_);

        int width = 0, height = 0, nChannels = 0;
        bool storageAllocated = false;
        for (unsigned int i = 0; i < 6; i++)
        {
            stbi_set_flip_vertically_on_load(false); // Cubemaps usually don't need vertical flipping

            unsigned char *data = stbi_load(faceFiles[i].c_str(), &width, &height, &nChannels, 0);
            if (! data)
            {
                spdlog::error("Cubemap tex failed to load at path: {}", faceFiles[i]);
                stbi_image_free(data);
                return false; // Return false if loading fails
            }

            spdlog::debug("Cubemap tex {} loading successful.", faceFiles[i]);
            //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            //            0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            //);
            
            // Determine source format based on channels
            GLenum dataFormat = (nChannels == 4) ? GL_RGBA : GL_RGB;

            if (!storageAllocated) {
                format_ = (nChannels == 4) ? GL_RGBA8 : GL_RGB8;
                width_ = width;
                height_ = height;

                spdlog::debug("Cubemap texture nChannel: {}, width: {}, height: {}", 
                    nChannels, width_, height_);

                // Allocate immutable storage for the cube map (6 faces, 1 mip level)
                // 1. Allocate immutable storage for the 6 faces (1 mip level in this example)
                glTextureStorage2D(mapId_, 1, format_, width_, height_);

                glTextureParameteri(mapId_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTextureParameteri(mapId_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTextureParameteri(mapId_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTextureParameteri(mapId_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTextureParameteri(mapId_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                storageAllocated = true;
            }

            glTextureSubImage3D(
                    mapId_,            // Texture ID
                    0,                 // Mipmap level
                    0, 0, i,           // xoffset, yoffset, zoffset (zoffset maps to face index 0–5)
                    width, height, 1,  // width, height, depth (depth = 1 for a single face layer)
                    dataFormat,            // Pixel format of your source data
                    GL_UNSIGNED_BYTE,  // Pixel data type
                    data      // Pointer to pixel data
            );

            stbi_image_free(data);
        }

        return true; // Return true if loading is successful
    }



    /*
    std::string faces[6] = {
        "right.jpg",  // GL_TEXTURE_CUBE_MAP_POSITIVE_X
        "left.jpg",   // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
        "top.jpg",    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
        "bottom.jpg", // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        "front.jpg",  // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
        "back.jpg"    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    */

    //unsigned char* facePixels[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

};