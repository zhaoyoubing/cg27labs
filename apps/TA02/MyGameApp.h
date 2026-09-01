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

        // ================ Create object entity ================
        // 1. Create the mesh object entity id
        objId_ = ecsWorld_.createEntityID();
        ecsWorld_.addComp<TransformComp>(objId_, TransformComp{});

        // ================ Create camera entity ================
        // 2. Create the camera entity id
        EntityID camId = ecsWorld_.createEntityID();

        ecsWorld_.addComp<CameraComp>(camId, CameraComp{
            .eye = glm::vec3(0.0f, 0.0f, 2.0f), // also eye position
            .fov = 60.0f,
            .front = glm::vec3(0, 0, -1),
            .yaw = -90.0f,
            .pitch = 0.0f,
            .viewport = Viewport(0, 0, mainWin_->getWidth(), mainWin_->getHeight())
        });

        // ================ Model Setup ================
        // 3. set up data and vertex buffers
        // std::shared_ptr<MeshModel> = GltfMeshModelLoader::loadModel();
        std::shared_ptr<Mesh> mesh = MeshFactory::createPyramid(1.0, 1.0);
        std::shared_ptr<MeshBufferGPU> meshBuf = std::make_shared<MeshBufferGPU>();
        meshBuf->createAndUploadBuffers(mesh);
        MeshHandle hMesh = meshMgr_.add(meshBuf);
        
        ecsWorld_.addComp<MeshComp>(objId_, MeshComp {hMesh});

        // ================ Shaders and Material Setup ================
        // 4. Load individual shader stages from disk
        Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_2a.vert");
        Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");

        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        std::shared_ptr<GPUPipeline> plainPipeline = std::make_shared<GPUPipeline>(shaderStages);

        Material mat(plainPipeline);
        mat.shadingModel = ShadingModel::Plain;
        // you should add material parameters here

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
    //EntityID playerId_;
    std::shared_ptr<Mesh> mesh_;
};