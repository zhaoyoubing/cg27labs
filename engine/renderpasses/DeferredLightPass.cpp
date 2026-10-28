#include "renderpasses/DeferredLightPass.h"



bool DeferredLightPass::init(RenderContext & ctx) {
    glCreateBuffers(1, &lightSSBO);

    
    glNamedBufferData(
        lightSSBO,
        MAX_LIGHTS * sizeof(GPULight),
        nullptr,
        GL_DYNAMIC_DRAW
    );
 
    return true;

}


void DeferredLightPass::execute(RenderContext & ctx) {
    glBindFramebuffer(GL_FRAMEBUFFER, gBuf);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawQuad(ctx, camera_.getViewMatrix(), camera_.getProjMatrix() );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void DeferredLightPass::setLights(const std::vector<GPULight> & lights)
{
    constexpr size_t MAX_LIGHTS = 128;

    size_t count = std::min(lights.size(), MAX_LIGHTS);

    if (count > 0)
    {
        glNamedBufferSubData(
            lightSSBO, 0, count * sizeof(GPULight),
            lights.data()
        );
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

    gpuPipe->setInt("uLightCount", count);
}

void DeferredLightPass::drawQuad(RenderContext& context) {
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}