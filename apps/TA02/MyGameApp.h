#pragma once

#include "GameApp.h"

class MyGameApp : GameApp {

public:

    MyGameApp(std:string name):GameApp(name) {

    }

    bool initRender() ovverride {

        // ================ Shaders and Pipeline Setup ================
        // Load individual shader stages from disk
        Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
        Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
        
        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        GPUPipeline basicPipeline(shaderStages);

        renderer().addPass(
            std::make_unique<ForwardPass>(std::move(basicPipeline)));
                
    }

   void update(float dt) override {

   }

private:

    // systems
    PlayerMoveSystem playerSys_;
    CameraSystem camSys_;
};