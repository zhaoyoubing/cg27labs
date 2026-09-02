#include "renderpasses/ForwardPassECS.h"

#include "entity/CameraComp.h"
#include "entity/TransformComp.h"
#include "entity/MaterialComp.h"
#include "entity/MeshComp.h"

void ForwardPassECS::execute(RenderContext& context)
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the scene graph instead

    auto view = context.scene_->ecsWorld_.view<TransformComp, MeshComp, MaterialComp>();



    for (auto entity : view)
    {
        const auto& transform =
            context.scene_->ecsWorld_.getComp<TransformComp>(entity);

        const auto& meshComp =
            context.scene_->ecsWorld_.getComp<MeshComp>(entity);

        const auto& matComp =
           context.scene_->ecsWorld_.getComp<MaterialComp>(entity);

        
        std::shared_ptr<MeshBufferGPU> meshBuf = meshComp.meshBuf;

        if (! meshBuf)
            continue;

        std::shared_ptr<Material> material = context.matMgr_.get(matComp.hMat);

        // Select the material's shader
        std::shared_ptr<GPUPipeline> gpuPipe = material->gpuPipe;

        assert (gpuPipe != nullptr);

        gpuPipe->bind();

        gpuPipe->setMat4("uModel", transform.getLocalMatrix());
        gpuPipe->setMat4("uView",  camera.getViewMatrix());
        gpuPipe->setMat4( "uProj", camera.getProjMatrix());

        //material.gpuPipe->setVec3( "uBaseColour",  material.baseColour);

        meshBuf->bind();
        meshBuf->draw();
        meshBuf->unbind();

        gpuPipe->unbind();
    }
}