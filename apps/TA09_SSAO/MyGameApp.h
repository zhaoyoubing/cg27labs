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
#include "renderpasses/SkyBoxPass.h"
#include "renderpasses/CubeMapReflectionPass.h"
#include "renderpasses/GBufferPass.h"
#include "renderpasses/DeferredLightPass.h"

#include "renderpasses/SSAOPass.h"
#include "renderpasses/SSAOBoxBlurPass.h"
#include "renderpasses/SSAODeferredLightPass.h"

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
        shaderMgr_.load("skybox", "shaders/skybox.vert", "shaders/skybox.frag");
        shaderMgr_.load("cubemap_reflect", "shaders/cubemap_reflect.vert", "shaders/cubemap_reflect.frag");


        shaderMgr_.load("deferred_gbuffer", "shaders/deferred_gbuffer.vert", "shaders/deferred_gbuffer.frag");
        shaderMgr_.load("deferred_lighting", "shaders/deferred_lighting.vert", "shaders/deferred_lighting.frag");
        shaderMgr_.load("ssao", "shaders/fullscreen.vert", "shaders/ssao.frag");
        shaderMgr_.load("boxblur", "shaders/fullscreen.vert", "shaders/boxblur.frag");
        shaderMgr_.load("ssao_deferred_lighting", "shaders/fullscreen.vert", "shaders/ssao_deferred_lighting.frag");


        // ================ Model Setup ================
        // 3. set up data and vertex buffers
        AABB bbox;
        glm::vec3 center;
        glm::vec3 dim;
        /*
        //std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf("assets/BoxTextured/glTF/BoxTextured.gltf", texMgr_,  matMgr_, shaderMgr_);
        std::unique_ptr<SceneNode> mesh = MeshFactory::loadGltf( //"assets/barrel/scene.gltf", 
                                                             "assets/box_normal/box_normal_tangent.gltf",
                                                                texMgr_,  matMgr_, shaderMgr_);
        scene_->root_ = std::move(mesh);
        */

        /*
        std::unique_ptr<SceneNode> teapot = MeshFactory::loadGltf( "assets/teapot_texblue/scene.gltf", 
                                                        texMgr_,  matMgr_, shaderMgr_);
        
        teapot->scaleToFitBBox(0.3);
        teapot->center();
        //teapot->trans.pos += glm::vec3(-0.1, 0.2, -0.3);
        scene_->root_->children.push_back(std::move(teapot));
        

        std::unique_ptr<SceneNode> bunny = MeshFactory::loadGltf("assets/bunny_tex.gltf", 
                                                             texMgr_,  matMgr_, shaderMgr_);
        bunny->scaleToFitBBox(0.5);
        bunny->trans.pos += glm::vec3(0.0, 0.3, 0.3);
        
        AABB bbox1 = bunny->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox1.min.x, bbox1.min.y, bbox1.min.z, 
                                                                    bbox1.max.x, bbox1.max.y, bbox1.max.z);
        scene_->root_->children.push_back(std::move(bunny));
        */
        
        std::unique_ptr<SceneNode> sponza = MeshFactory::loadGltf("assets/Sponza/glTF/Sponza.gltf", 
                                                             texMgr_,  matMgr_, shaderMgr_);
        sponza->scaleToFitBBox(30.0);
        //bbox = sponza->getBBox();
        //center = bbox.getCenter();
        //sponza->centerAt(glm::vec3(center.x, center.y, bbox.min.z));
        sponza->center();
        scene_->root_->children.push_back(std::move(sponza));
        
        /*
        std::unique_ptr<SceneNode> chapel = MeshFactory::loadGltf("assets/chapel_ruin/scene.gltf", 
                                                             texMgr_,  matMgr_, shaderMgr_);
        chapel->scaleToFitBBox(10.0);
        chapel->center();
        scene_->root_->children.push_back(std::move(chapel));
        */
          

        bbox = scene_->getBBox();
        spdlog::debug("Bounding Box min: {} {} {}, max: {} {} {}", bbox.min.x, bbox.min.y, bbox.min.z, 
                                                                    bbox.max.x, bbox.max.y, bbox.max.z);
        dim = bbox.getDimension();
        center = bbox.getCenter();
        spdlog::debug("Scene centre: {} {} {}", center.x, center.y, center.z);
        float maxSize = std::max(dim.x, std::max(dim.y, dim.z) );
        
        // ================ Create camera entity ================
        // 4. Create the camera entity id
        EntityID camId = scene_->ecsWorld_.createEntityID();

        scene_->ecsWorld_.addComp<CameraComp>(camId, CameraComp{
            //.eye = glm::vec3(0.0f, 2.0f, 0.0f),
            .eye = center, //  + glm::vec3(0.0f, 0.0f, 2.5f) * maxSize, 
            //.eye = center  + glm::vec3(0.0f, 0.0f, 2.5f) * maxSize, 
            .fov = 60.0f,
            .near = 0.1f,
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

        initLights(maxSize * 1.0f, 20); // add 10 random lights around the scene
        
        /*
        // ================ Skybox Setup ================
        std::vector<std::string> faces = {
            "assets/cubemapYokohama/posx.jpg",  // right
            "assets/cubemapYokohama/negx.jpg",  // left
            "assets/cubemapYokohama/posy.jpg",  // top
            "assets/cubemapYokohama/negy.jpg",  // bottom
            "assets/cubemapYokohama/posz.jpg",  // front
            "assets/cubemapYokohama/negz.jpg"   // back
        };
        scene_->skybox_.load(faces);
        */


        // ================ Render Pipeline Setup ================
        // 6. render pipeline init
        //renderPipe_.addPass(std::make_unique<ForwardPassLightNormal>());
        //renderPipe_.addPass(std::make_unique<CubeMapReflectionPass>());
        //renderPipe_.addPass(std::make_unique<SkyBoxPass>());
        
        renderPipe_.addPass(std::make_unique<GBufferPass>());
        renderPipe_.addPass(std::make_unique<SSAOPass>());
        renderPipe_.addPass(std::make_unique<SSAOBoxBlurPass>());
        renderPipe_.addPass(std::make_unique<SSAODeferredLightPass>());
        
        renderPipe_.init(context_);
    };

    // add many random lights around the scene, for testing deferred rendering
    void initLights(float radius, int nLights = 20) {

        //radius = radius * 1.5f;

        for (unsigned int i = 0; i < nLights; i++)
        {
            LightComp light {
                .type = POINT,
                .pos = glm::vec3(radius, radius, radius),
                .dir = glm::vec3(0.0f, 0.0f, -1.0f),
                .colour = glm::vec3(1.0f, 1.0f, 1.0f),
                .intensity = 2.0f,
                .a = 1.0f,
                .b = 0.09f,
                .c = 0.032f 
            };

            // calculate slightly random offsets
            float theta = static_cast<float>(((rand() % 360) / 360.0) * 3.1415926 * 2);

            float x = radius * cos(theta);
            float z = radius * sin(theta);

            float y = static_cast<float>( ( ((rand() % 100) / 50.0) - 1.0) * radius );

            light.pos = glm::vec3(x, y, z);
            
            // also calculate random color
            float r = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
            float g = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
            float b = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
            //light.colour = glm::vec3(r, g, b);

            scene_->lightList.push_back(light);
        }
    }

   void tick(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.tick(scene_->ecsWorld_, objId_, mainWin_->getInputState(), dt);
        camSys_.tick(scene_->ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();

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