#include "renderpasses/SSRBlendPass.h"

bool SSRBlendPass::init(RenderContext & ctx) {
    spdlog::debug("SSRBlendPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("ssr_blend");

    if (! gpuPipe) return false;

    // 2. fetch the camera and set the viewport size
    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    bInited = true;
    return bInited;
}


void SSRBlendPass::execute(RenderContext & ctx) {

    camera_ = ctx.scene_->getMainCamera();

    // 1. set onscreen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. OpenGL setting
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //glClear(GL_COLOR_BUFFER_BIT);

    // 3. bind the shader
    gpuPipe->bind();

    // 4. set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    // 5. set uniforms
    gpuPipe->setMat4("uMatView", camera_.getViewMatrix());
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    // 6. set the lighting texure and the reflection texxture
    glBindTextureUnit(8, ctx.postp_.texScene);
    glBindTextureUnit(9, ctx.ssr_.texSSR);

    // 7. draw the fullscreen proxy triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}
