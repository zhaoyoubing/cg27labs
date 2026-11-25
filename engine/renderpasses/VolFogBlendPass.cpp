#include "renderpasses/VolFogBlendPass.h"

bool VolFogBlendPass::init(RenderContext & ctx) {
    spdlog::debug("SSRBlendPass::init()");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("volfog_blend");

    if (! gpuPipe) return false;

    // 2. fetch the camera and set the viewport size
    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("Viewport width {}, height {}", width_, height_);

    bInited = true;
    return bInited;
}


void VolFogBlendPass::execute(RenderContext & ctx) {

    camera_ = ctx.scene_->getMainCamera();

    // 1. set onscreen rendering
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. OpenGL setting
    glViewport(0, 0, width_, height_);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // 3. bind the shader
    gpuPipe->bind();


    // 3. set uniforms
    //gpuPipe->setFloat("uVolFogIntensty", ctx.volFog_.blendIntensity);
    gpuPipe->setFloat("uVolFogIntensty", 2.0f);


    // 4. set the lighting texure and the reflection texxture
    glBindTextureUnit(8, ctx.postp_.texScene);
    glBindTextureUnit(9, ctx.volFog_.mapVolFog);

    // 5. draw the fullscreen proxy triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}
