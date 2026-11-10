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
    /*
    glCreateTextures(GL_TEXTURE_2D, 1, & texBufA);
    glTextureStorage2D(texBufA, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texBufA, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texBufA, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    */

    glCreateTextures(GL_TEXTURE_2D, 1, & texBufB);
    glTextureStorage2D(texBufB, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texBufB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texBufB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // initial binding
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufB, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // share blur texture ids
    //ctx.postp_.bloomBufferA = texBufA;
    texBufA = ctx.postp_.bloomBufferA;
    ctx.postp_.bloomBufferB = texBufB;

    return true;
}


void GaussianBlurPass::execute(RenderContext & ctx) {

    // read camera data
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT);

    gpuPipe->bind();

    for (int i = 0; i < nIter; i++)
    {
        int pass = i % 2;
        gpuPipe->setInt("uPass", pass);

        // set GBuffer textures to the shader
        if (pass == 0) {
            glBindTextureUnit(8, texBufA);
            glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufB, 0);
            spdlog::trace("Bloom bright pass rendering to {}", texBufB);
        }
        else {
            glBindTextureUnit(8, texBufB);
            glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texBufA, 0);
            spdlog::trace("Bloom bright pass rendering to {}", texBufA);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }


    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
