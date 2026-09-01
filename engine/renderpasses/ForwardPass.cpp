#include "renderpasses/ForwardPass.h"

#include "entity/CameraComp.h"
#include "entity/TransformComp.h"
#include "entity/MaterialComp.h"
#include "entity/MeshComp.h"

void ForwardPass::execute(RenderContext& context)
{
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // read camera data
    auto viewCam = context.registry_.view<CameraComp>();

    // a default camera in case no primary camera is found
    CameraComp camera;
    for (auto entity : viewCam)
    {
        auto & cam = context.registry_.getComp<CameraComp>(entity);
        if (cam.isPrimary) {
            camera = cam;
            break;
        }
    }

    glViewport(0, 0, camera.viewport.w, camera.viewport.h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto view = context.registry_.view<
        TransformComp, MeshComp, MaterialComp>();

    for (auto entity : view)
    {
        const auto& transform =
            context.registry_.getComp<TransformComp>(entity);

        const auto& meshComp =
            context.registry_.getComp<MeshComp>(entity);

        const auto& matComp =
           context.registry_.getComp<MaterialComp>(entity);

        
        std::shared_ptr<MeshBufferGPU> meshBuf = context.meshMgr_.get(meshComp.hMesh);

        if (! meshBuf)
            continue;

        Material& material = context.matMgr_.get(matComp.hMat);

        // Select the material's shader
        material.gpuPipe->bind();

        material.gpuPipe->setMat4("uModel", transform.getLocalMatrix());
        material.gpuPipe->setMat4("uView",  camera.getViewMatrix());
        material.gpuPipe->setMat4( "uProj", camera.getProjMatrix());

        //material.gpuPipe->setVec3( "uBaseColour",  material.baseColour);

        meshBuf->bind();
        meshBuf->draw();
        meshBuf->unbind();

        material.gpuPipe->unbind();
    }
}