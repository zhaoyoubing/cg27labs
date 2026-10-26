#include "renderpasses/ShadowDepthPass.h"

bool ShadowDepthPass::init(RenderContext& context) {
    spdlog::debug("ShadowDepthPass::init()");

    // 1. Fetch shader from manager
    depthShader = context.shaderMgr_.get("shadowdepth");

    if (!depthShader) return false;

    /*
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
    
    // disable drawing the colour buffer
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);

    // 4. Validate FBO
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    */

    return true; // Everything is built and ready for the render loop!
}

void ShadowDepthPass::execute(RenderContext & ctx) {

    //spdlog::debug("ShadowDepthPass::execute()");

    /*
    // read camera data
    auto viewCam = ctx.scene_->ecsWorld_.view<CameraComp>();

    // a default camera in case no primary camera is found
    for (auto entity : viewCam)
    {
        auto & cam = ctx.scene_->ecsWorld_.getComp<CameraComp>(entity);
        if (cam.isPrimary) {
            camera_ = cam;
            break;
        }
    }
    */

    // get the default single light from the scene
    // need to pack into light data structure in the future
    auto viewLight = ctx.scene_->ecsWorld_.view<LightComp>();
    for (auto entity : viewLight)
    {
        auto & light = ctx.scene_->ecsWorld_.getComp<LightComp>(entity);
        lightDefault_ = light;
        break;
    }

    glm::mat4 matLightView = glm::lookAt(lightDefault_.pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); 
    glm::mat4 matLightProj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 1500.0f);
    //glm::mat4 matLightProj = glm::ortho(-80.0f,80.0f,-80.0f,80.0f, 0.0f, 300.0f);
    // glm::mat4 matLightViewProj = matLightProj * matLightView;


    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    //glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    //glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Pass-specific states for shadow rendering
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT); // Cull front faces for shadow bias adjustment

    depthShader->bind();
    
    // Use light space matrices
    drawSceneGraph(ctx, ctx.scene_->root_, glm::mat4(1.0), matLightView, matLightProj);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowDepthPass::drawSceneGraph(const RenderContext & ctx, std::unique_ptr<SceneNode>& node, 
                const glm::mat4& parentMatrix, 
                const glm::mat4& viewMatrix, 
                const glm::mat4& projMatrix ) 
{
   // 1. Calculate this node's world transform
    glm::mat4 worldMatrix = parentMatrix * node->trans.getLocalMatrix();

    // 2. If this node has geometry, issue the OpenGL draw call
    for (auto & mesh : node->meshList)
    {
        std::shared_ptr<MeshBufferGPU> meshBuf = mesh->geometry_->gpuBuffer;

        assert(meshBuf != nullptr);

        std::shared_ptr<GPUPipeline> gpuPipe = depthShader;
        gpuPipe->bind();

        // set shader uniforms
        gpuPipe->setMat4("uModel", worldMatrix);
        gpuPipe->setMat4("uLightView", viewMatrix);
        gpuPipe->setMat4("uLightProj", projMatrix);
        
        // no need to use this
        // gpuPipe->setVec3("uViewPos", lightDefault_.pos);
        
        // draw the mesh
        meshBuf->bind();
        meshBuf->draw();
        meshBuf->unbind();

        gpuPipe->unbind();
    }

    // 3. Recursively process all children, passing down the updated world matrix
    for (auto& child : node->children) {
        drawSceneGraph(ctx, child, worldMatrix, viewMatrix, projMatrix);
    }

}