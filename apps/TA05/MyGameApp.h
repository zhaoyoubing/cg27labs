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
#include "renderpasses/ForwardPassLight.h"
#include "renderpasses/ShadowDepthPass.h"
#include "renderpasses/ShadowLightPass.h"

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
        shaderMgr_.load("shadowdepth", "shaders/shadowdepth.vert", "shaders/shadowdepth.frag");
        shaderMgr_.load("shadowlight", "shaders/shadowlight.vert", "shaders/shadowlight.frag");

        // ================ Model Setup ================
        // 3. set up data and vertex buffers
        //std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/BoxTextured/glTF/BoxTextured.gltf", texMgr_,  matMgr_, shaderMgr_);
        std::unique_ptr<SceneNode> bunny = MeshFactory::loadGltf(//"assets/box_normal/box_normal_tangent.gltf",
                                                             "assets/bunny_tex.gltf", 
                                                             texMgr_,  matMgr_, shaderMgr_);
        //scene_->root_ = std::move(mesh);
        bunny->scaleToFitBBox(0.5);
        bunny->trans.pos += glm::vec3(0.0, 0.2, 0.3);
        
        AABB bbox1 = bunny->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox1.min.x, bbox1.min.y, bbox1.min.z, 
                                                                    bbox1.max.x, bbox1.max.y, bbox1.max.z);
        scene_->root_->children.push_back(std::move(bunny));

        std::unique_ptr<SceneNode> barrel = MeshFactory::loadGltf("assets/barrel/scene.gltf", 
                                                             texMgr_,  matMgr_, shaderMgr_);
        bbox1 = barrel->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox1.min.x, bbox1.min.y, bbox1.min.z, 
                                                                    bbox1.max.x, bbox1.max.y, bbox1.max.z);
        barrel->scaleToFitBBox(0.8);
        barrel->center();
        barrel->trans.pos = barrel->trans.pos + glm::vec3(-0.7, 0.0, 0.0);
        scene_->root_->children.push_back(std::move(barrel));


        std::unique_ptr<SceneNode> teapot = MeshFactory::loadGltf( "assets/teapot_texblue/scene.gltf", 
                                                        texMgr_,  matMgr_, shaderMgr_);
        
        teapot->scaleToFitBBox(0.3);
        teapot->center();
        teapot->trans.pos += glm::vec3(0.5, 0.4, 0.6);
        scene_->root_->children.push_back(std::move(teapot));


        AABB bbox = scene_->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox.min.x, bbox.min.y, bbox.min.z, 
                                                                    bbox.max.x, bbox.max.y, bbox.max.z);
        glm::vec3 dim = bbox.getDimension();
        glm::vec3 center = bbox.getCenter();
        float maxSize = bbox.getMaxDimLen();;

        // ================ Create camera entity ================
        // 4. Create the camera entity id
        EntityID camId = scene_->ecsWorld_.createEntityID();

        scene_->ecsWorld_.addComp<CameraComp>(camId, CameraComp{
            .eye = center + glm::vec3(0.0f, 0.0f, 1.5f) * maxSize,
            .fov = 45.0f,
            .far = maxSize * 3.0f,
            .front = glm::vec3(0.0, 0, -1),
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
        std::unique_ptr<ShadowDepthPass> shadowDepthPass = std::make_unique<ShadowDepthPass>();
        std::unique_ptr<ShadowLightPass> shadowLightPass = std::make_unique<ShadowLightPass>();
        ShadowDepthPass * depthPass = shadowDepthPass.get();
        ShadowLightPass * lightPass = shadowLightPass.get();
        renderPipe_.addPass(std::move(shadowDepthPass));
        //renderPipe_.addPass(std::move(shadowLightPass));
        //renderPipe_.addPass(std::make_unique<ForwardPassLightNormal>());

        /*
        RenderContext ctx {
            .scene_ = scene_,
            .meshMgr_ = meshMgr_,
            .matMgr_ = matMgr_,
            .texMgr_ = texMgr_,
            .shaderMgr_ = shaderMgr_
        };
        */

        // init render passes in the render pipeline
        renderPipe_.init(context_);
        // chain two passes
        //lightPass->setShadowMap(depthPass->getShadowMap());
    };

   void tick(float dt) override {
        // 3. Run systems using window's delta time directly
        // where is the best place to update viewports ? at the moment it is in Camera System
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