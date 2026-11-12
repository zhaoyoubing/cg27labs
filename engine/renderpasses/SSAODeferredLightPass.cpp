#include "renderpasses/SSAODeferredLightPass.h"


bool SSAODeferredLightPass::init(RenderContext & ctx) {

    spdlog::debug("SSAO Deferred Lighting Pass init");

    gpuPipe = ctx.shaderMgr_.get("ssao_deferred_lighting");

    if (nullptr == gpuPipe) return false;

    bInited = true;
    return bInited;
}


void SSAODeferredLightPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    glBindTextureUnit(9, ctx.ssao_.texBlurAO);

    gpuPipe->setVec3("uEyePos", camera_.eye);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);

    setPointLights(ctx);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}

void SSAODeferredLightPass::setPointLights(RenderContext & ctx)
{
    std::vector<LightComp> & lights = ctx.scene_->lightList;
    spdlog::trace("DeferredLightPass::setPointLights() - light count {}", lights.size());

    gpuPipe->setInt("uLightCount", static_cast<int>(lights.size()));

    for (int i = 0; i < lights.size(); i++)
    {
        LightComp & light = lights[i];

        
        spdlog::trace("Light pos {}, {}, {}, colour {}, {}, {}, intensity {}", 
            light.pos.x, light.pos.y, light.pos.z,
            light.colour.r, light.colour.g, light.colour.b,
            light.intensity
        );
        

        gpuPipe->setVec3("uLights[" + std::to_string(i) + "].pos", light.pos);
        gpuPipe->setVec3("uLights[" + std::to_string(i) + "].colour", light.colour);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].intensity", light.intensity);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].a", light.a);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].b", light.b);
        gpuPipe->setFloat("uLights[" + std::to_string(i) + "].c", light.c);
    }
}
