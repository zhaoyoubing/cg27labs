#include "renderpasses/SSAOBoxBlurPass.h"

bool SSAOBoxBlurPass::init(RenderContext & ctx) {
    spdlog::debug("SSAOBoxBlurPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("boxblur");

    if (! gpuPipe) return false;

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    // to get viewport width and height
    // currently viewport is in camera, to be changed in the future
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    // configure framebuffer
    // ------------------------------
    glCreateFramebuffers(1, &FBO);
    
    // two textures for ping-pong Gaussian blurring
    glCreateTextures(GL_TEXTURE_2D, 1, & texBuf);
    glTextureStorage2D(texBuf, 1, GL_RGBA16F, width_, height_);
    glTextureParameteri(texBuf, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texBuf, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent texture bleeding across screen edges
    glTextureParameteri(texBuf, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texBuf, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // initial binding
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBuf, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer not complete!");
       return false;
    }

    spdlog::debug("In Box Blur Pass init, framebuffer texture {}", texBuf);

    // share blur texture ids
    // use previous rendering
    //texBufA = ctx.postp_.bloomBufferA;
    ctx.ssao_.texBlur = texBuf;

    bInited = true;
    return bInited;
}

// warning: don't call glClear(GL_COLOR_BUFFER_BIT) in rendering
// it may lead to undesired clearing and blackscreen
void SSAOBoxBlurPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    // read camera data
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    gpuPipe->bind();

    //glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    //glBindTextureUnit(1, ctx.gbuf_.gNormal);
    //glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    glBindTextureUnit(9, ctx.ssao_.texAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
