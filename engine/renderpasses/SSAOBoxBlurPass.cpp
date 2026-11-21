#include "renderpasses/SSAOBoxBlurPass.h"

bool SSAOBoxBlurPass::init(RenderContext & ctx) {
    spdlog::debug("SSAOBoxBlurPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("ssao_boxblur");

    if (! gpuPipe) return false;

    // 2. fetch the camera from the scene
    camera_ = ctx.scene_->getMainCamera();

    // set framebuffer width and height
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    // 3. generate framebuffer
    // ------------------------------
    glCreateFramebuffers(1, &FBO);
    
    // 4. create the framebuffer texture (blurred AO map)
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    glTextureStorage2D(texFBO, 1, GL_RGBA16F, width_, height_);
    glTextureParameteri(texFBO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent texture bleeding across screen edges
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 5. bind the render target
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFBO, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // 6. finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("Framebuffer not complete!");
       return false;
    }

    spdlog::debug("In Box Blur Pass init, framebuffer texture {}", texFBO);

    // 7. share blurred AO map in the RenderContext
    ctx.ssao_.mapBlurAO = texFBO;

    bInited = true;
    return bInited;
}

void SSAOBoxBlurPass::execute(RenderContext & ctx) {
    if (!bInited) return;


    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen rendering
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);   // onscreen rendering for debugging

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    gpuPipe->bind();

    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    glBindTextureUnit(9, ctx.ssao_.mapAO);

    gpuPipe->setInt("uRadius", 2);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
