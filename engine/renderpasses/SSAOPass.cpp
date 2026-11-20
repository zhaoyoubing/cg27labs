#include "renderpasses/SSAOPass.h"

// for sample generation
#include <random>
#include <cmath>

bool SSAOPass::init(RenderContext & ctx) {

    spdlog::debug("SSAO Pass init");

    // 1. fetch the shader
    gpuPipe = ctx.shaderMgr_.get("ssao");

    if (nullptr == gpuPipe) return false;

    // 2. get the camera and framebuffer size
    camera_ = ctx.scene_->getMainCamera();
    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    // 3. create the framebuffer
    glCreateFramebuffers(1, &FBO);

    // 4. create the frambuffer texure (AO map)
    glCreateTextures(GL_TEXTURE_2D, 1, & texFBO);
    // 4.1 allocate single-channel 8-bit uncompressed storage
    glTextureStorage2D(texFBO, 1, GL_R8, width_, height_);

    // 4.2 set texture parameters
    glTextureParameteri(texFBO, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texFBO, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 5. associate the texture with the framebuffer
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texFBO, 0);

    // 5.1 and tell OpenGL the attachments to render to
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // 6. finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // 7. generate hemisphere sample vectors
    samplePoints_ = generateSamples(nSample);

    // 8. share AO map id in the render contenxt
    ctx.ssao_.mapAO = texFBO;

    bInited = true;
    return bInited;
}


void SSAOPass::execute(RenderContext & ctx) {
    if (!bInited) return;

    spdlog::trace("SSAO Pass: albedo {}", ctx.gbuf_.gAlbedo);

    // fetch camera for view port
    // as window resizing has not been supported this year
    camera_ = ctx.scene_->getMainCamera();
    
    // bind the offscreen framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // offscreen render
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // onscreen render for debugging
    
    // render setup 
    // white represents fully unoccluded space
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    // render parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // bind the shaders
    gpuPipe->bind();

    // bind GBuffer textures in the shader
    glBindTextureUnit(0, ctx.gbuf_.gAlbedo);
    glBindTextureUnit(1, ctx.gbuf_.gNormal);
    glBindTextureUnit(2, ctx.gbuf_.gMat);
    glBindTextureUnit(3, ctx.gbuf_.gDepth);

    // set shader uniforms
    // set sample parameters
    gpuPipe->setInt("uKernelSize", nSample);
    gpuPipe->setFloat("uRadius", ctx.ssao_.radius);
    gpuPipe->setFloat("uBias", ctx.ssao_.bias);
    gpuPipe->setFloat("uPowerAO", ctx.ssao_.power);

    // set view and projection matrices
    gpuPipe->setMat4("uMatView", camera_.getViewMatrix());
    gpuPipe->setMat4("uMatProj", ctx.gbuf_.matProj);
    gpuPipe->setMat4("uMatInvProj", ctx.gbuf_.matInvProj);
    
    // set sample kernel vectors
    // this is not an efficent way of setting an array
    // need to add vector setting API in GPUPipeline
    for (int i = 0; i < nSample; i++)
        gpuPipe->setVec3("uSamplePoints[" + std::to_string(i) + "]", samplePoints_[i]);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    gpuPipe->unbind();

}

// code from GenAI
// generate the sample vectors (points) inside a hemisphere, i.e. the sample kernel
// a sample size of 64 is used in this tutorial
std::vector<glm::vec3> SSAOPass::generateSamples(int nsample) {
    std::vector<glm::vec3> samples;
    samples.reserve(nsample);

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
