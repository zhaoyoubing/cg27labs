#include "renderpasses/DeferredLightPass.h"


bool DeferredLightPass::init(RenderContext & ctx) {

    spdlog::debug("Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("deferred_lighting");

    if (nullptr == gpuPipe) return false;

    
    glCreateBuffers(1, &lightSSBO);
/*
    glNamedBufferData(
        lightSSBO,
        MAX_LIGHTS * sizeof(GPULight),
        nullptr,
        GL_DYNAMIC_DRAW
    );
*/

    glCreateVertexArrays(1, &screenVAO);

    return true;
}


void DeferredLightPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

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

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    gpuPipe->unbind();

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