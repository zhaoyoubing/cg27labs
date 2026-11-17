#include "renderpasses/BloomBrightPass.h"

bool BloomBrightPass::init(RenderContext & ctx) {
    spdlog::debug("BloomBrightPass::init()");

    // 1. Fetching the shader pipeline from the shader manager
    gpuPipe = ctx.shaderMgr_.get("bloom_brightpass");

    if (! gpuPipe) return false;

    // 2. get camera from scene for viewport width and height
    camera_ = ctx.scene_->getMainCamera();

    // set up framebuffer dimension, downsize for better blurring
    int downSampleFactor = ctx.postp_.downSampleFactor;
    width_ = camera_.viewport.w / downSampleFactor;
    height_ = camera_.viewport.h / downSampleFactor;

    // for full window preview in debugging
    // width_ = camera_.viewport.w;
    // height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    // 3. create scene render framebuffer FBO id
    // ------------------------------
    glCreateFramebuffers(1, &FBO);
    
    // create framebuffer colour texture id
    glCreateTextures(GL_TEXTURE_2D, 1, & texFB);
    glTextureStorage2D(texFB, 1, GL_RGBA16F, width_, height_);
    glTextureParameteri(texFB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Prevent texture bleeding across screen edges
    glTextureParameteri(texFB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // 4. asssociate the texture to framebuffer attachments 
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFB, 0);

    // tell OpenGL the colour attachments to render to
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // 5. finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // share the framebuffer texture
    ctx.postp_.bloomBufferA = texFB;
    spdlog::debug("After Bloom Bright Pass init, bloomBuffer A {}", ctx.postp_.bloomBufferA);

    //spdlog::debug("After BloomRender init, texRender {}", texScene);

    bInited = true;
    return bInited;

}


void BloomBrightPass::execute(RenderContext & ctx) {

    spdlog::trace("Bloom bright pass rendering to {}", texFB);

    // bind the framebuffer
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // onscreen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen rendering

    glViewport(0, 0, width_, height_);

    // depth and face culling disabled for fullscreen rendering
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // use the (fullscreen.vert, bloom_brightpass.frag) shader pipeline
    gpuPipe->bind();

    // threshold for highlight filtering
    gpuPipe->setFloat("uThreshold", ctx.postp_.bloomThreshold);

    // bind the scene texture in the shader pipeline (OpenGL 4.2+)
    glBindTextureUnit(8, ctx.postp_.texScene);

    // draw a fullscreen triangle for simplicity
    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
