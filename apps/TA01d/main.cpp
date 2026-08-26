#include <iostream>
#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "render/Pipeline.h"
#include "drawColourVertex.h"

#include "ecs/EcsTypes.h"
#include "ecs/components/TransformComp.h"
#include "ecs/components/CameraComp.h"
#include "ecs/systems/CameraSystem.h"

#include "ui/Window.h"
#include "ui/InputState.h"
#include "PlayerMoveSystem.h"


int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    spdlog::set_level(spdlog::level::debug);

    // ================ GLFW and Glad Setup ================
    // 1. Initialize GLFW Window and Input Context
    Window window(800, 800, "TA01d");

    // 2. Initialize ECS World Pool
    ECSWorldPool ecsWorld;
    PlayerMoveSystem moveSys;
    CameraSystem camSys;

    // ================ Create object entity ================
    EntityID objId = ecsWorld.createEntityID();
    ecsWorld.addComp<TransformComp>(objId, TransformComp{});

    // ================ Create player entity ================
    // 1. Create the player entity id
    EntityID player = ecsWorld.createEntityID();

    // 2. Attach a single TransformCompto it
    ecsWorld.addComp<TransformComp>(player, TransformComp{
        .pos = glm::vec3(0.0f, 0.0f, 2.0f) // also eye position
    });

    // 3. Attach a single CameraComp to the player (First Person View)
    ecsWorld.addComp<CameraComp>(player, CameraComp{
        .fov = 45.0f,
        .front = glm::vec3(0, 0, -1),
        .yaw = -90.0f,
        .pitch = 0.0f
    });

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    Pipeline basicPipeline(shaderStages);
    

    // ================ Model Setup ================
    // set up data and vertex buffers
    initColourVertex();

    // ================ Rendering Mode Setup ================
    glPolygonMode(GL_FRONT, GL_FILL);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); 
    glEnable(GL_DEPTH_TEST);

    // ================ Main Render and Event Loop ================
    while (! window.shouldClose()) {
        // 1. Poll events from the OS
        window.pollEvents();

        // 2. Update the frame clock/delta time
        window.updateDeltaTime();

        float dt = window.getDeltaTime();

        // 3. Run systems using window's delta time directly
        moveSys.update(ecsWorld, objId, window.getInputState(), dt);
        camSys.update(ecsWorld, window.getInputState(), dt);

        // 4. Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader
        // the order is always TRS (translate, rotate, scale) for the modelview matrix
        // for rotation we choose to rotate around the local x-axis first, then the y-axis
        TransformComp& trans = ecsWorld.getComp<TransformComp>(objId);
        glm::mat4 mat_model = glm::translate(trans.pos)  
            * glm::rotate(glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) 
            * glm::scale(glm::vec3(1.0f));
        
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

        drawColourVertex();


        window.swapBuffers();

        // set mouse x and y offsets to 0
        window.clearInputState();
    }


    return 0;
}