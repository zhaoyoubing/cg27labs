#include "renderpasses/SSRReflectPass.h"


bool SSRReflectPass::init(RenderContext & ctx) {

    spdlog::debug("Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("ssr_reflect");

    if (nullptr == gpuPipe) return false;

    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    glCreateFramebuffers(1, &FBO);

    // generate SSR texture
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    // Allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texFBO, 1, GL_RGBA16F, width_, height_);

    // Set filtering to nearest or linear depending on whether you blur it
    glTextureParameteri(texFBO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFBO, 0);

    // 4. tell OpenGL the colour attachments to render to
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    ctx.ssr_.texSSR = texFBO;
    
    bInited = true;
    return bInited;
}


void SSRReflectPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);   // onscreen
    
    glViewport(0, 0, width_, height_);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    //gpuPipe->setInt("uWidth", width_);
    //gpuPipe->setInt("uHeight", height_);

    gpuPipe->setMat4("uMatView", camera_.getViewMatrix());
    gpuPipe->setMat4("uMatProj", ctx.gbuf_.matProj);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    gpuPipe->setFloat("uMaxRayDistance", ctx.ssr_.maxRayDistance);
    gpuPipe->setFloat("uMaxSteps", ctx.ssr_.maxSteps);
    gpuPipe->setFloat("uStride", ctx.ssr_.stride);
    gpuPipe->setFloat("uThickness", ctx.ssr_.thickness);
    gpuPipe->setFloat("uRoughThres", ctx.ssr_.roughThres);


    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}



