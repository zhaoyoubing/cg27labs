#include "renderpasses/SSAOPass.h"

// for sample generation
#include <random>
#include <cmath>

bool SSAOPass::init(RenderContext & ctx) {

    spdlog::debug("SSAO Pass init");

    gpuPipe = ctx.shaderMgr_.get("ssao");

    if (nullptr == gpuPipe) return false;

    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    glCreateFramebuffers(1, &FBO);

    // generate SSAO texture
    glCreateTextures(GL_TEXTURE_2D, 1, & texSSAO);
    // Allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texSSAO, 1, GL_R8, width_, height_);

    // Set filtering to nearest or linear depending on whether you blur it
    glTextureParameteri(texSSAO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texSSAO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texSSAO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texSSAO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texSSAO, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    samplePoints_ = generateSamples(nSample);

    ctx.ssao_.texAO = texSSAO;

    bInited = true;
    return bInited;
}


void SSAOPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("SSAO Pass: albedo {}", ctx.gbuf_.gAlbedo);

    // read camera data
    camera_ = ctx.scene_->getMainCamera();

    //glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    gpuPipe->bind();

    // set GBuffer textures to the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    gpuPipe->setInt("uKernelSize", nSample);
    gpuPipe->setFloat("uRadius", 1.0);
    gpuPipe->setFloat("uBias", 0.0032);

    //gpuPipe->setVec2("uViewport", glm::vec2(width_, height_));

    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);
    gpuPipe->setMat4("uMatProj", glm::inverse(ctx.gbuf_.matInvProj));

    // this is not an efficent way of setting an array
    for (int i = 0; i < nSample; i++)
        gpuPipe->setVec3("uSamplePoints[" + std::to_string(i) + "]", samplePoints_[i]);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}

// code from GenAI
std::vector<glm::vec3> SSAOPass::generateSamples(int nsample) {
    std::vector<glm::vec3> samples;
    samples.reserve(64);

    // Standard uniform random number generators
    std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
    std::default_random_engine generator;

    for (int i = 0; i < nsample; ++i) {
        // Generate random sample inside a unit hemisphere (Z is positive/upwards)
        glm::vec3 sample(
            randomFloats(generator) * 2.0f - 1.0f, // X: [-1.0, 1.0]
            randomFloats(generator) * 2.0f - 1.0f, // Y: [-1.0, 1.0]
            randomFloats(generator)                // Z: [ 0.0, 1.0] (hemisphere)
        );
        
        sample = glm::normalize(sample);
        
        // Scale random strength so that samples are clustered closer to the center/origin
        float scale = (float)i / nsample;
        scale = std::lerp(0.1f, 1.0f, scale * scale); // Non-linear distribution
        sample *= scale;

        samples.push_back(sample);
    }
    return samples;
}
