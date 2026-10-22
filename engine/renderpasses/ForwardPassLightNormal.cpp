#include "renderpasses/ForwardPassLightNormal.h"

#include "entity/CameraComp.h"
#include "entity/TransformComp.h"
#include "entity/MaterialComp.h"
#include "entity/MeshComp.h"
#include "entity/LightComp.h"


void ForwardPassLightNormal::execute(RenderContext& context)
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // read camera data
    auto viewCam = context.scene_->ecsWorld_.view<CameraComp>();

    // a default camera in case no primary camera is found
    for (auto entity : viewCam)
    {
        auto & cam = context.scene_->ecsWorld_.getComp<CameraComp>(entity);
        if (cam.isPrimary) {
            camera_ = cam;
            break;
        }
    }

    // get the default single light from the scene
    // need to pack into light data structure in the future
    auto viewLight = context.scene_->ecsWorld_.view<LightComp>();
    for (auto entity : viewLight)
    {
        auto & light = context.scene_->ecsWorld_.getComp<LightComp>(entity);
        lightDefault_ = light;
        break;
    }

    glViewport(0, 0, camera_.viewport.w, camera_.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSceneGraph(context.scene_->root_, glm::mat4(1.0), 
                context, camera_.getViewMatrix(), camera_.getProjMatrix());

}


void ForwardPassLightNormal::drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         const RenderContext& ctx,
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
    MaterialManager & matMgr = ctx.matMgr_;

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
        } else if (ShadingModel::PlainBlinn == material->shadingModel) {
            gpuPipe = ctx.shaderMgr_.get("plainblinn");
        } else if (ShadingModel::TexBlinnNormal == material->shadingModel) {
            gpuPipe = ctx.shaderMgr_.get("texblinn_normal");
        } else {
            gpuPipe = ctx.shaderMgr_.get("plain");
        }

        assert(gpuPipe != nullptr);

        material->bind(gpuPipe);

        // set shader uniforms
        gpuPipe->setMat4("uModel", worldMatrix);
        gpuPipe->setMat4("uView",  viewMatrix);
        gpuPipe->setMat4("uProj", projMatrix);
        
        gpuPipe->setVec3("uViewPos", camera_.eye);

        gpuPipe->setVec3("uLight.dir", lightDefault_.dir);
        gpuPipe->setVec3("uLight.pos", lightDefault_.pos);
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
        drawSceneGraph(child, worldMatrix, ctx, viewMatrix, projMatrix);
    }
}
