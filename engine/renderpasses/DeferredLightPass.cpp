#include "renderpasses/DeferredLightPass.h"


bool DeferredLightPass::init(RenderContext & ctx) {

    spdlog::debug("Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("deferred_lighting");

    if (nullptr == gpuPipe) return false;

/*    
    glCreateBuffers(1, &lightSSBO);

    // for light data, we will use a shader storage buffer object (SSBO) to store an array of lights

    glNamedBufferData(
        lightSSBO,
        MAX_LIGHTS * sizeof(GPULight),
        nullptr,
        GL_DYNAMIC_DRAW
    );
*/

    bInited = true;
    return bInited;
}


void DeferredLightPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    gpuPipe->setVec3("uEyePos", camera_.eye);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    setPointLights(ctx);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}

void DeferredLightPass::setPointLights(RenderContext & ctx)
{
    std::vector<LightComp> & lights = ctx.scene_->lightList;
    spdlog::trace("DeferredLightPass::setPointLights() - light count {}", lights.size());

    gpuPipe->setInt("uLightCount", static_cast<int>(lights.size()));

    for (int i = 0; i < lights.size(); i++)
    {
        LightComp & light = lights[i];

        
        spdlog::trace("Light pos {}, {}, {}, colour {}, {}, {}, intensity {}", 
            light.pos.x, light.pos.y, light.pos.z,
            light.colour.r, light.colour.g, light.colour.b,
            light.intensity
        );
        

        gpuPipe->setVec3("uLights[" + std::to_string(i) + "].pos", light.pos);
        gpuPipe->setVec3("uLights[" + std::to_string(i) + "].colour", light.colour);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].intensity", light.intensity);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].a", light.a);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].b", light.b);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].c", light.c);
    }
}

void DeferredLightPass::setSSBOLights(const std::vector<GPULight> & lights)
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

/*
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
    */