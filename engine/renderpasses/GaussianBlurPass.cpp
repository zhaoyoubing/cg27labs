#include "renderpasses/GaussianBlurPass.h"

bool GaussianBlurPass::init(RenderContext & ctx) {
    spdlog::debug("GaussianBlurPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("gaussian_blur");

    if (! gpuPipe) return false;

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    // to get viewport width and height
    // currently viewport is in camera, to be changed in the future
    // shrinking for better blurring
    int downSampleFactor = ctx.postp_.downSampleFactor;
    width_ = camera_.viewport.w / downSampleFactor;
    height_ = camera_.viewport.h / downSampleFactor;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    // configure framebuffer
    // ------------------------------
    glCreateFramebuffers(1, &FBO);
    
    // two textures for ping-pong Gaussian blurring

    glCreateTextures(GL_TEXTURE_2D, 1, & texBufB);
    glTextureStorage2D(texBufB, 1, GL_RGBA16F, width_, height_);
    glTextureParameteri(texBufB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texBufB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent texture bleeding across screen edges
    glTextureParameteri(texBufB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texBufB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // initial binding
    //glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufB, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    // if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //    spdlog::error("Framebuffer not complete!");
    //    return false;
    //}

    spdlog::debug("In Gaussian Pass init, bloomBuffer A {}", ctx.postp_.bloomBufferA);

    // share blur texture ids
    // use previous rendering
    texBufA = ctx.postp_.bloomBufferA;
    ctx.postp_.bloomBufferB = texBufB;

    bInited = true;
    return bInited;
}

// warning: don't call glClear(GL_COLOR_BUFFER_BIT) in rendering
// it may lead to undesired clearing and blackscreen
void GaussianBlurPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    // read camera data
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    gpuPipe->bind();

    for (int i = 0; i < ctx.postp_.blurPassCount; i++)
    {
        int pass = i % 2;
        gpuPipe->setInt("uPass", pass);

        // set GBuffer textures to the shader
        if (pass == 0) {
            glBindTextureUnit(8, texBufA);
            glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufB, 0);
            spdlog::trace("Bloom blur pass A rendering to B {}", texBufB);
        }
        else {
            glBindTextureUnit(8, texBufB);
            glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufA, 0);
            spdlog::trace("Bloom blur pass B rendering to A {}", texBufA);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }


    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
