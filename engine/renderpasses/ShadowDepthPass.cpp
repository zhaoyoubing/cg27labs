#include "renderpasses/ShadowDepthPass.h"

bool ShadowDepthPass::init(RenderContext& context) {
    // 1. Fetch shader from manager
    depthShader = context.shaderMgr_.get("shadow_depth");
    if (!depthShader) return false;

    // 2. Allocate FBO and Texture memory
    glGenFramebuffers(1, & shadowFBO);
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, 
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
                 
    // Set texture parameters (filtering, wrapping)...
    
    // 3. Attach to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // 4. Validate FBO
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true; // Everything is built and ready for the render loop!
}

void ShadowDepthPass::execute(RenderContext & ctx) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Pass-specific states for shadow rendering
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT); // Cull front faces for shadow bias adjustment

    depthShader->bind();
    drawSceneGraph(ctx, ctx.scene_->root_);
}

void ShadowDepthPass::drawSceneGraph(const RenderContext & ctx, std::unique_ptr<SceneNode>& node, 
                const glm::mat4& parentMatrix, 
                const glm::mat4& viewMatrix, 
                const glm::mat4& projMatrix ) 
{


}