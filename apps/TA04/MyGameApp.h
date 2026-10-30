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
#include "entity/LightComp.h"

#include "renderpasses/ForwardPassLightNormal.h"

#include <algorithm>

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

        // ================ Shaders and Material Setup ================
        // 2. Load default shader stages from disk
        shaderMgr_.load("plain", "shaders/vcolour.vert", "shaders/vcolour.frag");
        shaderMgr_.load("texplain", "shaders/vtexture.vert", "shaders/vtexture.frag");
        shaderMgr_.load("texblinn", "shaders/blinn_texture.vert", "shaders/blinn_texture.frag");
        shaderMgr_.load("texblinn_normal", "shaders/texblinn_normal.vert", "shaders/texblinn_normal.frag");

        // ================ Model Setup ================
        // 3. set up data and vertex buffers
        // std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/BoxTextured/glTF/BoxTextured.gltf", texMgr_,  matMgr_, shaderMgr_);
        std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf( //"assets/barrel/scene.gltf", 
                                                             "assets/box_normal/box_normal_tangent.gltf",
                                                                texMgr_,  matMgr_, shaderMgr_);
        scene_->root_ = std::move(mesh);

        AABB bbox = scene_->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox.min.x, bbox.min.y, bbox.min.z, 
                                                                    bbox.max.x, bbox.max.y, bbox.max.z);
        glm::vec3 dim = bbox.getDimension();
        glm::vec3 center = bbox.getCenter();
        float maxSize = std::max(dim.x, std::max(dim.y, dim.z) );

        // ================ Create camera entity ================
        // 4. Create the camera entity id
        EntityID camId = scene_->ecsWorld_.createEntityID();

        scene_->ecsWorld_.addComp<CameraComp>(camId, CameraComp{
            .eye = center + glm::vec3(0.0f, 0.0f, 2.5f) * maxSize, // + glm::vec3(-200.0, 5.0, 0) for barrel testing
            .fov = 45.0f,
            .far = maxSize * 5.0f,
            .front = glm::vec3(0, 0, -1),
            .yaw = -90.0f,
            .pitch = 0.0f,
            .viewport = Viewport(0, 0, mainWin_->getWidth(), mainWin_->getHeight())
        });

        // ================ Light Setup ================
        // 5. Create a directional light
        EntityID lightId = scene_->ecsWorld_.createEntityID();
        scene_->ecsWorld_.addComp<LightComp>(lightId, LightComp{
            .type = POINT,
            .pos = bbox.max * 3.0f,
            .dir = glm::vec3(0.0f, -1.0f, 0.0f),
            .colour = glm::vec3(1.0f, 1.0f, 1.0f),
            .intensity = 1.0f,
            .a = 1.0f,
            .b = 0.09f,
            .c = 0.032f 
        });

        // ================ Shaders and Pipeline Setup ================
        // 6. render pipeline init
        renderPipe_.addPass(std::make_unique<ForwardPassLightNormal>());
    };

   void tick(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.tick(scene_->ecsWorld_, objId_, mainWin_->getInputState(), dt);
        camSys_.tick(scene_->ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();

        /*
        RenderContext ctx {
            .scene_ = scene_,
            .meshMgr_ = meshMgr_,
            .matMgr_ = matMgr_,
            .texMgr_ = texMgr_,
            .shaderMgr_ = shaderMgr_
        };
        */

        renderPipe_.render(context_);

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