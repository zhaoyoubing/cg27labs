#pragma once

#include "GameApp.h"

class MyGameApp : GameApp {

public:

    MyGameApp(std:string name):GameApp(name) {

    }

    bool initRenderPipeline() ovverride {

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
        playerSys_.update(ecsWorld, objId, window.getInputState(), dt);
        camSys_.update(ecsWorld, window.getInputState(), dt);
   }

   void render() override {
        // Clear frame buffers
        mainWin_->clearScreen();

        renderPipe_.render(RenderContext {
            .registry = ecsWorld_;
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
};