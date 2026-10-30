#include "renderpasses/GBufferPass.h"

bool GBufferPass::init(RenderContext & ctx) {
    spdlog::debug("Before GBuffer init");

    // 1. Fetch shader from manager
    gpuPipe = ctx.shaderMgr_.get("deferred_gbuffer");

    if (! gpuPipe) return false;

    // to get viewport width and height
    // currently viewport is in camera, to be changed in the future

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

    width_ = camera_.viewport.w;
    height_ = camera_.viewport.h;

    spdlog::debug("GBuffer width {}, height {}", width_, height_);

    // configure g-buffer framebuffer
    // ------------------------------
    glCreateFramebuffers(1, &gBufFBO);
    //glBindFramebuffer(GL_FRAMEBUFFER, gBufFBO);
    
    /*
    // vertex position buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &gPos);
    glTextureStorage2D(gPos, 1, GL_RGBA16F, width_, height_);
    glTextureParameteri(gPos, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gPos, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(gBufFBO, GL_COLOR_ATTACHMENT0, gPos, 0);
    */

    // colour + specular color buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &gAlbedo);
    glTextureStorage2D(gAlbedo, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(gAlbedo, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gAlbedo, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(gBufFBO, GL_COLOR_ATTACHMENT0, gAlbedo, 0);


    // vertex normal colour buffer
    glCreateTextures(GL_TEXTURE_2D, 1, &gNormal);
    glTextureStorage2D(gNormal, 1, GL_RGBA16F, width_, height_);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width_, height_, 0, GL_RGBA, GL_FLOAT, NULL);
    glTextureParameteri(gNormal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gNormal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    glNamedFramebufferTexture(gBufFBO, GL_COLOR_ATTACHMENT1, gNormal, 0);

    // Material: metallic roughness 
    glCreateTextures(GL_TEXTURE_2D, 1, &gMat);
    glTextureStorage2D(gMat, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(gMat, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gMat, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(gBufFBO, GL_COLOR_ATTACHMENT2, gMat, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glNamedFramebufferDrawBuffers(gBufFBO, 3, attachments);

    glCreateTextures(GL_TEXTURE_2D, 1, &gDepth);
    glTextureStorage2D(gDepth, 1, GL_DEPTH_COMPONENT24, width_, height_);
    glTextureParameteri(gDepth, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gDepth, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(gBufFBO, GL_DEPTH_ATTACHMENT, gDepth, 0 );
    
    assert (glIsFramebuffer(gBufFBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(gBufFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // share GBuffer texture ids
    ctx.gbuf_.gAlbedo = gAlbedo;
    ctx.gbuf_.gNormal = gNormal;
    ctx.gbuf_.gMat = gMat;
    ctx.gbuf_.gDepth = gDepth;

    spdlog::debug("After GBuffer init, albedo {}, normal {}, depth {}", gAlbedo, gNormal, gDepth);

    return true;

}


void GBufferPass::execute(RenderContext & ctx) {

    if (!bInited) return;

    spdlog::trace("GBuffer rendering, albedo {}", gAlbedo);

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


    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, gBufFBO);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
    //glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSceneGraph(ctx, ctx.scene_->root_, glm::mat4(1.0), 
             camera_.getViewMatrix(), camera_.getProjMatrix());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBufferPass::drawSceneGraph(RenderContext& ctx, std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix) {
    
    // 1. Calculate this node's world transform
    glm::mat4 worldMatrix = parentMatrix * node->trans.getLocalMatrix();

    // 2. If this node has geometry, issue the OpenGL draw call
    for (auto & mesh : node->meshList)
    {
        std::shared_ptr<MeshBufferGPU> meshBuf = mesh->geometry_->gpuBuffer;
        assert(meshBuf != nullptr);

        std::shared_ptr<Material> material = mesh->material_;
        assert(material != nullptr);

        material->bind(gpuPipe);

        // set shader uniforms
        gpuPipe->setMat4("uModel", worldMatrix);
        gpuPipe->setMat4("uView", viewMatrix);
        gpuPipe->setMat4("uProj", projMatrix);
                
        // draw the mesh
        meshBuf->bind();
        meshBuf->draw();
        meshBuf->unbind();

        material->unbind();
    }

    // 3. Recursively process all children, passing down the updated world matrix
    for (auto& child : node->children) {
        drawSceneGraph(ctx, child, worldMatrix,viewMatrix, projMatrix);
    }

}