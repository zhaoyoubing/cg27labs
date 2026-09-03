#include "renderpasses/ForwardPass.h"

#include "entity/CameraComp.h"
#include "entity/TransformComp.h"
#include "entity/MaterialComp.h"
#include "entity/MeshComp.h"

void ForwardPass::execute(RenderContext& context)
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // read camera data
    auto viewCam = context.scene_->ecsWorld_.view<CameraComp>();

    // a default camera in case no primary camera is found
    CameraComp camera;
    for (auto entity : viewCam)
    {
        auto & cam = context.scene_->ecsWorld_.getComp<CameraComp>(entity);
        if (cam.isPrimary) {
            camera = cam;
            break;
        }
    }

    glViewport(0, 0, camera.viewport.w, camera.viewport.h);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSceneGraph(context.scene_->root_, glm::mat4(1.0), 
                context.matMgr_, camera.getViewMatrix(), camera.getProjMatrix());

}

void ForwardPass::drawSceneGraph(std::unique_ptr<SceneNode>& node, 
                         const glm::mat4& parentMatrix, 
                         const MaterialManager & matMgr,
                         const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
    // 1. Calculate this node's world transform
    glm::mat4 worldMatrix = parentMatrix * node->trans.getLocalMatrix();

    // 2. If this node has geometry, issue the OpenGL draw call
    //std::shared_ptr<MaterialMesh> mesh = node->meshList;

    for (auto & mesh : node->meshList)
    {
        std::shared_ptr<MeshBufferGPU> meshBuf = mesh->geometry_->gpuBuffer;

        assert(meshBuf != nullptr);

        std::shared_ptr<Material> material = mesh->material_;

        assert(material != nullptr);

        std::shared_ptr<GPUPipeline> gpuPipe = material->gpuPipe;

        assert(gpuPipe != nullptr);

        std::shared_ptr<Texture> texture = material->baseColourTex;

        assert(texture != nullptr);

        // Select the material's shader
        gpuPipe->bind();

        gpuPipe->setMat4("uModel", worldMatrix);
        gpuPipe->setMat4("uView",  viewMatrix);
        gpuPipe->setMat4("uProj", projMatrix);

        texture->bind(0);
        gpuPipe->setInt("baseColourMap", 0);
        
        //material.gpuPipe->setVec3( "uBaseColour",  material.baseColour);

        meshBuf->bind();
        meshBuf->draw();
        meshBuf->unbind();

        texture->unbind();
        gpuPipe->unbind();
    }

    // 3. Recursively process all children, passing down the updated world matrix
    for (auto& child : node->children) {
        drawSceneGraph(child, worldMatrix, matMgr, viewMatrix, projMatrix);
    }
}
