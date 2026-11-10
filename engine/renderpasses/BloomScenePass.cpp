#include "renderpasses/BloomScenePass.h"

bool BloomScenePass::init(RenderContext & ctx) {
    spdlog::debug("Before GBuffer init");

    // 1. Fetch shader from manager
    // gpuPipe = ctx.shaderMgr_.get("deferred_gbuffer");
    // if (! gpuPipe) return false;

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
    glCreateFramebuffers(1, &FBO);
    //glBindFramebuffer(GL_FRAMEBUFFER, gBufFBO);
    

    // colour + specular color buffer
    glCreateTextures(GL_TEXTURE_2D, 1, & texScene);
    glTextureStorage2D(texScene, 1, GL_RGBA8, width_, height_);
    glTextureParameteri(texScene, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texScene, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, texScene, 0);

    // tell OpenGL which colour attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(FBO, 1, attachments);

    assert (glIsFramebuffer(FBO) == GL_TRUE);

    // finally check if framebuffer is complete
    if ( glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("GBuffer Framebuffer not complete!");
        return false;
    }

    // share GBuffer texture ids
    ctx.postp_.texScene = texScene;

    //ctx.gbuf_.matInvProj = glm::inverse(camera_.getProjMatrix());

    spdlog::debug("After BloomRender init, texRender {}", texScene);

    return true;

}


void BloomScenePass::execute(RenderContext & ctx) {

    if (!bInited) return;

    spdlog::trace("Bloom scene rendering,  {}", texScene);

    // read camera data
    camera_ = ctx.scene_->getMainCamera();
    std::vector<LightComp> lights = ctx.scene_->getLights();
    if (! lights.empty())
        lightDefault_ = lights[0];

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    glViewport(0, 0, width_, height_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSceneGraph(ctx, ctx.scene_->root_, glm::mat4(1.0), 
             camera_.getViewMatrix(), camera_.getProjMatrix());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BloomScenePass::drawSceneGraph(RenderContext& ctx, std::unique_ptr<SceneNode>& node, 
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


        std::shared_ptr<GPUPipeline> gpuPipe = nullptr;
        if (ShadingModel::TexBlinn == material->shadingModel) {
            gpuPipe = ctx.shaderMgr_.get("texblinn");
            spdlog::trace("Using texblinn shader");
        } else if (ShadingModel::PlainBlinn == material->shadingModel) {
            gpuPipe = ctx.shaderMgr_.get("plainblinn");
        } else if (ShadingModel::TexBlinnNormal == material->shadingModel) {
            gpuPipe = ctx.shaderMgr_.get("texblinn_normal");
            spdlog::trace("Using texblinn_normal shader");
        } else {
            gpuPipe = ctx.shaderMgr_.get("plain");
        }
        material->bind(gpuPipe);

        // set shader uniforms
        gpuPipe->setMat4("uModel", worldMatrix);
        gpuPipe->setMat4("uView", viewMatrix);
        gpuPipe->setMat4("uProj", projMatrix);

        gpuPipe->setVec3("uViewPos", camera_.eye);

        //gpuPipe->setInt("uLight.type", lightDefault_.type);
        gpuPipe->setVec3("uLight.pos", lightDefault_.pos);
        gpuPipe->setVec3("uLight.dir", lightDefault_.dir);
        gpuPipe->setVec3("uLight.colour", lightDefault_.colour);
        gpuPipe->setFloat("uLight.intensity", lightDefault_.intensity);
        gpuPipe->setFloat("uLight.a", lightDefault_.a);
        gpuPipe->setFloat("uLight.b", lightDefault_.b);
        gpuPipe->setFloat("uLight.c", lightDefault_.c);
                
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