#include "renderpasses/BloomBlendPass.h"

bool BloomBlendPass::init(RenderContext & ctx) {
    spdlog::debug("GaussianBlurPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("bloom_blend");

    if (! gpuPipe) return false;

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    // to get viewport width and height
    // currently viewport is in camera, to be changed in the future
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    return true;
}


void BloomBlendPass::execute(RenderContext & ctx) {

    // read camera data
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //glClear(GL_COLOR_BUFFER_BIT);

    gpuPipe->bind();

    gpuPipe->setFloat("uExposure", ctx.postp_.exposure);
    gpuPipe->setFloat("uBloomIntensity", ctx.postp_.bloomIntensity);

    glBindTextureUnit(8, ctx.postp_.texScene);
    glBindTextureUnit(9, ctx.postp_.bloomBufferB);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}
