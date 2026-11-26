#include "renderpasses/VolumetricFogPass.h"


bool VolumetricFogPass::init(RenderContext & ctx) {

    spdlog::debug("SSR Reflection Pass init");

    gpuPipe = ctx.shaderMgr_.get("volumetric_fog");

    if (nullptr == gpuPipe) return false;

    // fetch the camera and set the framebuffer size
    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    // temporarily set the directional light for testing volumetric fog
    auto viewLight = ctx.scene_->ecsWorld_.view<LightComp>();
    for (auto entity : viewLight)
    {
        auto & light = ctx.scene_->ecsWorld_.getComp<LightComp>(entity);
        lightDefault_ = light;
        break;
    }


    glCreateFramebuffers(1, &FBO);

    // generate SSR texture
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    // Allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texFBO, 1, GL_RGBA16F, width_, height_);

    // Set filtering to nearest or linear depending on whether you blur it
    glTextureParameteri(texFBO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFBO, 0);

    // 4. tell OpenGL the colour attachments to render to
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    ctx.volFog_.mapVolFog = texFBO;
    
    bInited = true;
    return bInited;
}


void VolumetricFogPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("Deferred Lighting: albedo {}", ctx.gbuf_.gAlbedo);

    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);   // onscreen for debugging
    
    // OpenGL settings
    glViewport(0, 0, width_, height_);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures in the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    // shadow map
    glBindTextureUnit(5, ctx.shadow_.depthMap); // for shadow testing
    glBindTextureUnit(8, ctx.postp_.texScene); // for debugging 

    // matrices need to reconstruct View Space positions
    gpuPipe->setVec3("uEyePos", camera_.eye);
    gpuPipe->setMat4("uMatView", camera_.getViewMatrix());
    gpuPipe->setMat4("uMatInvView", glm::inverse(camera_.getViewMatrix()));
    gpuPipe->setMat4("uMatProj", ctx.gbuf_.matProj);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);
    gpuPipe->setMat4("uMatLightSpace", ctx.shadow_.matLightSpace);


    // set the directional light
    gpuPipe->setVec3("uLight.pos", lightDefault_.pos);
    gpuPipe->setVec3("uLight.dir", lightDefault_.dir);
    gpuPipe->setVec3("uLight.colour", lightDefault_.colour);
    gpuPipe->setFloat("uLight.intensity", lightDefault_.intensity);


    // Volumetric Fog specific parameters
    gpuPipe->setFloat("uNumSteps", ctx.volFog_.numSteps);

     // draw the proxy triangle, no VAO needed
    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}



