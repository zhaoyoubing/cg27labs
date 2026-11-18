#include "renderpasses/SSRReflectPass.h"


bool SSRReflectPass::init(RenderContext & ctx) {

    spdlog::debug("Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("ssr_reflect");

    if (nullptr == gpuPipe) return false;

    glCreateFramebuffers(1, &FBO);

    // generate SSAO texture
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    // Allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texFBO, 1, GL_R8, width_, height_);

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

    ctx.ssr_.texReflect = texFBO;
    
    bInited = true;
    return bInited;
}


void SSRReflectPass::execute(RenderContext & ctx) {
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

    //gpuPipe->setVec3("uEyePos", camera_.eye);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}



