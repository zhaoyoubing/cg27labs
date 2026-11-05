#pragma once

#include "RenderPass.h"

#include <glad/glad.h>
#include <string>


class SkyBoxPass : public RenderPass {
public:
    explicit SkyBoxPass() { };

    bool init(RenderContext & ctx) override {
        gpuPipe = ctx.shaderMgr_.get("skybox");

        // 1. Create the VAO and VBO objects
        glCreateVertexArrays(1, &skyboxVAO);
        glCreateBuffers(1, &skyboxVBO);

        // 2. Allocate and upload vertex data into the VBO
        glNamedBufferData(skyboxVBO, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        // 3. Configure vertex attribute layout using DSA
        // Bind the VBO to vertex buffer binding point 0 with a stride of 3 floats (12 bytes)
        glVertexArrayVertexBuffer(skyboxVAO, 0, skyboxVBO, 0, 3 * sizeof(float));

        // Enable attribute index 0 (matching 'layout (location = 0) in vec3 aPos;' in your vertex shader)
        glEnableVertexArrayAttrib(skyboxVAO, 0);

        // Define format for attribute 0: 3 floats, not normalized, offset 0
        glVertexArrayAttribFormat(skyboxVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

        // Associate attribute index 0 with binding point 0
        glVertexArrayAttribBinding(skyboxVAO, 0, 0);

        return true;
    }

    void execute(RenderContext & ctx) override {
        // read camera data
        auto viewCam = ctx.scene_->ecsWorld_.view<CameraComp>();

        // a default camera in case no primary camera is found
        for (auto entity : viewCam)
        {
            auto & cam = ctx.scene_->ecsWorld_.getComp<CameraComp>(entity);
            if (cam.isPrimary) {
                camera_ = cam;
                break;
            }
        }

        gpuPipe->bind();
        gpuPipe->setMat4("uView", camera_.getViewMatrix());
        gpuPipe->setMat4("uProj", camera_.getProjMatrix());

        //glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
        
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL); 
        render(ctx);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        //glEnable(GL_DEPTH_TEST);

        gpuPipe->unbind();
    }

    void render(RenderContext & ctx) {
        // Bind the skybox shader and set uniforms
        // Bind the cubemap texture
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        // Set any additional uniforms for the skybox shader

        glBindVertexArray(skyboxVAO);
        ctx.scene_->skybox_.bind(5); // Bind the cubemap texture to texture unit 5
        //glBindTextureUnit(0, cubemap); // Assuming sampler is bound to unit 0
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

private:
    GLuint skyboxVAO;
    GLuint skyboxVBO;
    CameraComp camera_;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    // temporary hardcoded cube vertices for the skybox (36 vertices, 3 floats each)
    float skyboxVertices[108] = {
        // back face        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        // left face
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // right face
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        // front face
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // bottom face
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        // top face
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
};
