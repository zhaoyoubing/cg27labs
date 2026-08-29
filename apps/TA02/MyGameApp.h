#pragma once

#include "GameApp.h"

#include "PlayerMoveSystem.h"

#include "scene/MeshGeoFactory.h"
#include "scene/MeshGeometry.h"

#include "device/Shader.h"
#include "device/GPUPipeline.h"
#include "systems/CameraSystem.h"

#include <string>

class MyGameApp : public GameApp {

public:

    MyGameApp(std::string name):GameApp(name) {

    }

    void initScene() override {
        // ================ Create object entity ================
        objId = ecsWorld_.createEntityID();
        ecsWorld_.addComp<TransformComp>(objId, TransformComp{});

        // ================ Create player entity ================
        // 1. Create the player entity id
        player = ecsWorld_.createEntityID();

        // ================ Model Setup ================
        // set up data and vertex buffers
        // std::shared_ptr<MeshModel> = GltfMeshModelLoader::loadModel();
        std::shared_ptr<MeshGeometry> = MeshGeoFactory::createPyramid(1.0, 1.0);
    };

    void initRenderPipeline() override {

        // ================ Shaders and Pipeline Setup ================
        // Load individual shader stages from disk
        Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
        Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
        
        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        GPUPipeline basicPipeline(shaderStages);

        renderPipe_.addPass(
            std::make_unique<ForwardPass>(std::move(basicPipeline)));
                
    }

   void update(float dt) override {
        // 3. Run systems using window's delta time directly
        playerSys_.update(ecsWorld_, objId, mainWin_->getInputState(), dt);
        camSys_.update(ecsWorld_, mainWin_->getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();

        renderPipe_.render({
            .registry = ecsWorld_,
            .camera = camera_
        });

        /*
        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader
        // the order is always TRS (translate, rotate, scale) for the modelview matrix
        // the rotation is around the local axes (intrinsic) : x-axis first, then y-axis and z-axis
        TransformComp& trans = ecsWorld.getComp<TransformComp>(objId);
        glm::mat4 mat_model = trans.getLocalMatrix();
        
        basicPipeline.setMat4("matModel", mat_model);


        TransformComp & transPlayer = ecsWorld.getComp<TransformComp>(player);
        CameraComp & camera = ecsWorld.getComp<CameraComp>(player);

        // set up the view matrix for the camera
        glm::mat4 view = glm::lookAt(
            transPlayer.pos, // Camera position in world space
            transPlayer.pos + camera.front, // Look at target
            camera.up  // Up vector
        );

        glm::mat4 proj = glm::perspective(camera.fov, camera.aspect, camera.near, camera.far);

        // update the view matrix uniform in the shader
        basicPipeline.setMat4("matView", view);
        basicPipeline.setMat4("matProj", proj);

        //drawColourVertex();
        */

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