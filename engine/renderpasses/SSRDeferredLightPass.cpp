#include "renderpasses/SSRDeferredLightPass.h"


bool SSRDeferredLightPass::init(RenderContext & ctx) {

    spdlog::debug("Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("ssr_deferred_lighting");

    if (nullptr == gpuPipe) return false;

    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    glCreateFramebuffers(1, &FBO);

    // generate scene texture
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    // Allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texFBO, 1, GL_RGB16F, width_, height_);

    // Set filtering to nearest or linear depending on whether you blur it
    glTextureParameteri(texFBO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // bind the texture to the render target
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFBO, 0);
    // tell OpenGL the colour attachments (render targets) to render to
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer not complete!");
        return false;
    }

    // share the render output in the render context
    ctx.postp_.texScene = texFBO;

    bInited = true;
    return bInited;
}


void SSRDeferredLightPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("SSR Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    //glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);  // onscreen for debugging
    
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    gpuPipe->setVec3("uEyePos", camera_.eye);
    gpuPipe->setMat4("uMatInvView", glm::inverse(camera_.getViewMatrix()));
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    setPointLights(ctx);

    //glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}

void SSRDeferredLightPass::setPointLights(RenderContext & ctx)
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

/*
void SSRDeferredLightPass::setSSBOLights(const std::vector<GPULight> & lights)
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
    */