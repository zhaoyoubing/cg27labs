#pragma once

#include "GameApp.h"

#include "PlayerMoveSystem.h"

#include "scene/MeshFactory.h"
#include "scene/Mesh.h"

#include "device/Shader.h"
#include "device/GPUPipeline.h"
#include "systems/CameraSystem.h"

#include "entity/MeshComp.h"
#include "entity/MaterialComp.h"

#include <string>

class MyGameApp : public GameApp {

public:

    MyGameApp(std::string name):GameApp(name) {  }

protected:
    void initResources() override {
        spdlog::info("Initialising entity-component-system (ECS), scene, models ...");

        // ================ Create player entity ================
        // 1. Create the player entity id
        playerId_ = ecsWorld_.createEntityID();

        // ================ Create object entity ================
        objId_ = ecsWorld_.createEntityID();
        ecsWorld_.addComp<TransformComp>(objId_, TransformComp{});

        // ================ Model Setup ================
        // set up data and vertex buffers
        // std::shared_ptr<MeshModel> = GltfMeshModelLoader::loadModel();
        std::shared_ptr<Mesh> mesh = MeshFactory::createPyramid(1.0, 1.0);
        std::shared_ptr<MeshBufferGPU> meshBuf = std::make_shared<MeshBufferGPU>();
        meshBuf->createAndUploadBuffers(mesh);
        MeshHandle hMesh = meshMgr_.add(meshBuf);
        
        ecsWorld_.addComp<MeshComp>(objId_, MeshComp {hMesh});

        // ================ Shaders and Material Setup ================
        // Load individual shader stages from disk
        Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
        Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");

        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        std::shared_ptr<GPUPipeline> plainPipeline = std::make_shared<GPUPipeline>(shaderStages);

        Material mat(plainPipeline);
        mat.shadingModel = ShadingModel::Plain;
        MaterialHandle hMat = matMgr_.add(mat);
        ecsWorld_.addComp<MaterialComp>(objId_, MaterialComp{ hMat });


        // ================ Shaders and Pipeline Setup ================
        renderPipe_.addPass(std::make_unique<ForwardPass>());
    };

   void update(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.update(ecsWorld_, objId_, mainWin_->getInputState(), dt);
        camSys_.update(ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();


        RenderContext ctx {
            .registry_ = ecsWorld_,
            .camera_ = camera_,
            .meshMgr_ = meshMgr_,
            .matMgr_ = matMgr_,
            .texMgr_ = texMgr_,
        };

        renderPipe_.render(ctx);

        // Swap front and back buffers
        mainWin_->swapBuffers();
   }

private:

    // systems
    PlayerMoveSystem playerSys_;
    CameraSystem camSys_;

    EntityID objId_;
    EntityID playerId_;
    std::shared_ptr<Mesh> mesh_;
};