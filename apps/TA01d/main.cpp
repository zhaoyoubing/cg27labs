#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "render/Pipeline.h"
#include "drawColourVertex.h"

#include "entity/Entity.h"
#include "entity/TransformComp.h"
#include "systems/CameraSystem.h"

#include "ui/Window.h"
#include "ui/InputState.h"
#include "PlayerMoveSystem.h"


int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    // ================ GLFW and Glad Setup ================
    // 1. Initialize GLFW Window and Input Context
    Window window(800, 800, "TA01d");
    
    InputState::Init(window.GetNativeWindow());

    // 2. Initialize ECS World Pool
    ECSWorldPool ecsWorld;
    PlayerMoveSystem moveSys;
    CameraSystem camSys;

    // ================ Init Transform Component ================
    EntityID objId = ecsWorld.CreateEntityID();
    TransformComp trans;
    ecsWorld.AddComp<TransformComp>(objId, trans);

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_c.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    Pipeline basicPipeline(shaderStages);
    

    // ================ Model Setup ================
    // set up data and vertex buffers
    initColourVertex();

    // ================ Rendering Mode Setup ================
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
    glEnable(GL_DEPTH_TEST);

    // ================ Main Render and Event Loop ================
    while (! window.ShouldClose()) {
        // 1. Poll events from the OS
        window.PollEvents();

        // 2. Update the frame clock/delta time
        window.UpdateDeltaTime();

        float dt = window.GetDeltaTime();
        // 3. Run systems using window's delta time directly
        moveSys.Update(ecsWorld, objId, dt);
        camSys.Update(ecsWorld, dt);

        // 4. Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader
        // the order is always TRS (translate, rotate, scale) for the modelview matrix
        // for rotation we choose to rotate around the local x-axis first, then the y-axis
        TransformComp& trans = ecsWorld.GetComp<TransformComp>(objId);
        glm::mat4 mat_model = glm::translate(trans.pos)  
            * glm::rotate(glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) 
            * glm::scale(glm::vec3(1.0f));
        
        //mat_model = glm::mat4(1.0);
        basicPipeline.setMat4("matModel", mat_model);


        // update the view matrix uniform in the shader
        // basicPipeline.setMat4("matView", view);
        // basicPipeline.setMat4("matProj", proj);

        drawColourVertex();


        window.SwapBuffers();
    }


    return 0;
}