#pragma once

#include "GameApp.h"

#include "PlayerMoveSystem.h"

#include "scene/MeshFactory.h"
#include "scene/MeshGeometry.h"
#include "scene/Scene.h"

#include "device/Shader.h"
#include "device/GPUPipeline.h"
#include "systems/CameraSystem.h"

#include "entity/MeshComp.h"
#include "entity/MaterialComp.h"

#include "renderpasses/ForwardPass.h"

#include <string>

class MyGameApp : public GameApp {

public:

    MyGameApp(std::string name):GameApp(name) {  }

protected:
    void initResources() override {
        spdlog::info("Initialising entity-component-system (ECS), scene, models ...");

        // ================ Create object entity ================
        // 1. Create the mesh object entity id
        objId_ = scene_->ecsWorld_.createEntityID();
        scene_->ecsWorld_.addComp<TransformComp>(objId_, TransformComp{});

        // ================ Create camera entity ================
        // 2. Create the camera entity id
        EntityID camId = scene_->ecsWorld_.createEntityID();

        scene_->ecsWorld_.addComp<CameraComp>(camId, CameraComp{
            .eye = glm::vec3(0.0f, 0.0f, 10.0f), // also eye position
            .fov = 45.0f,
            .far = 1500.f,
            .front = glm::vec3(0, 0, -1),
            .yaw = -90.0f,
            .pitch = 0.0f,
            .viewport = Viewport(0, 0, mainWin_->getWidth(), mainWin_->getHeight())
        });

        // ================ Shaders and Material Setup ================
        // 3. Load individual shader stages from disk
        shaderMgr_.load("texture_plain", "shaders/vtexture.vert", "shaders/vtexture.frag");

        // ================ Model Setup ================
        // 4. set up data and vertex buffers
         std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/BoxTextured/glTF/BoxTextured.gltf", texMgr_,  matMgr_, shaderMgr_);
        //std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/BoxTextured/glTF-Binary/BoxTextured.glb", texMgr_,  matMgr_, shaderMgr_);
        //std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/bunny_tex.gltf", texMgr_,  matMgr_, shaderMgr_);
        scene_->root_ = std::move(mesh);
       
        // ================ Shaders and Pipeline Setup ================
        // 5. render pipline init
        renderPipe_.addPass(std::make_unique<ForwardPass>());
    };

   void tick(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.tick(scene_->ecsWorld_, objId_, mainWin_->getInputState(), dt);
        camSys_.tick(scene_->ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();

        RenderContext ctx {
            .scene_ = scene_,
            .meshMgr_ = meshMgr_,
            .matMgr_ = matMgr_,
            .texMgr_ = texMgr_,
            .shaderMgr_ = shaderMgr_
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

};