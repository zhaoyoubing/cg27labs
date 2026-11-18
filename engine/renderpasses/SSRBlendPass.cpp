#include "renderpasses/SSRBlendPass.h"

bool SSRBlendPass::init(RenderContext & ctx) {
    spdlog::debug("SSRBlendPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("ssr_blend");

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


void SSRBlendPass::execute(RenderContext & ctx) {

    camera_ = ctx.scene_->getMainCamera();

    // read camera data
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //glClear(GL_COLOR_BUFFER_BIT);

    gpuPipe->bind();

    gpuPipe->setMat4("uMatView", camera_.getViewMatrix());
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    glBindTextureUnit(8, ctx.postp_.texScene);
    glBindTextureUnit(9, ctx.ssr_.texSSR);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}
