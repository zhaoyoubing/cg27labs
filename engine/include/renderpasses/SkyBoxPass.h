#pragma once

#include "RenderPass.h"

#include <glad/glad.h>
#include <string>


class SkyBoxPass : public RenderPass {
public:
    SkyBoxPass();
    ~SkyBoxPass();

    bool init() {
        return true;
    }
    
    void render() {
        // Bind the skybox shader and set uniforms
        // Bind the cubemap texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        // Set any additional uniforms for the skybox shader
    }

private:
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    GLuint cubemap;
};
