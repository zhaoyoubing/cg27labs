#include "renderpasses/BloomBrightPass.h"

bool BloomBrightPass::init(RenderContext & ctx) {
    spdlog::debug("BloomBrightPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("bloom_brightpass");

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
    //glBindFramebuffer(GL_FRAMEBUFFER, gBufFBO);
    
    // colour + specular color buffer
    glCreateTextures(GL_TEXTURE_2D, 1, & texFB);
    glTextureStorage2D(texFB, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texFB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texFB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFB, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // share GBuffer texture ids
    ctx.postp_.bloomBufferA = texFB;

    //ctx.gbuf_.matInvProj = glm::inverse(camera_.getProjMatrix());

    //glCreateVertexArrays(1, &screenVAO);

    //spdlog::debug("After BloomRender init, texRender {}", texScene);

    return true;

}


void BloomBrightPass::execute(RenderContext & ctx) {

    spdlog::trace("Bloom bright pass rendering to {}", texFB);

    // read camera data
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT);

    gpuPipe->bind();

    gpuPipe->setFloat("uThreshold", ctx.postp_.bloomThreshold);

    // set GBuffer textures to the shader
    glBindTextureUnit(8, ctx.postp_.texScene);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
