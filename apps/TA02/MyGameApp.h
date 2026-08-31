#pragma once

#include "GameApp.h"

#include "PlayerMoveSystem.h"

#include "scene/MeshFactory.h"
#include "scene/Mesh.h"

#include "device/Shader.h"
#include "device/GPUPipeline.h"
#include "systems/CameraSystem.h"

#include <string>

class MyGameApp : public GameApp {

public:

    MyGameApp(std::string name):GameApp(name) {  }

protected:
    void initScene() override {
        spdlog::info("Initialising entity-component-system (ECS), scene, models ...");

        // ================ Create object entity ================
        objId = ecsWorld_.createEntityID();
        ecsWorld_.addComp<TransformComp>(objId, TransformComp{});

        // ================ Create player entity ================
        // 1. Create the player entity id
        player = ecsWorld_.createEntityID();

        // ================ Model Setup ================
        // set up data and vertex buffers
        // std::shared_ptr<MeshModel> = GltfMeshModelLoader::loadModel();
        std::shared_ptr<Mesh> mesh = MeshFactory::createPyramid(1.0, 1.0);
    };

    void initRenderPipeline() override {

        spdlog::info("Initialising the rendering peipeline  ...");
        
        // ================ Shaders and Pipeline Setup ================
        // Load individual shader stages from disk
        Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
        Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
        
        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        std::shared_ptr<GPUPipeline> basicPipeline = std::make_shared<GPUPipeline>(shaderStages);

        renderPipe_.addPass(std::make_unique<ForwardPass>(basicPipeline) );
             
    }

   void update(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.update(ecsWorld_, objId, mainWin_->getInputState(), dt);
        camSys_.update(ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();


        RenderContext ctx {
            .registry_ = ecsWorld_,
            .camera_ = camera_,
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

    EntityID objId;
    EntityID player;
};