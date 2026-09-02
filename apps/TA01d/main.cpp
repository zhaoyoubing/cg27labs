#include "drawColourVertex.h"
#include "PlayerMoveSystem.h"

#include "device/GPUPipeline.h"
#include "entity/EcsTypes.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"
#include "systems/CameraSystem.h"
#include "ui/Window.h"
#include "ui/InputState.h"

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <iostream>


int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    spdlog::set_level(spdlog::level::debug);

    // ================ GLFW and Glad Setup ================
    // 1. Initialize GLFW Window and Input Context
    Window window(800, 800, "TA01d");

    // 2. Initialize ECS World Pool
    ECSWorldRegistry ecsWorld;
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
        .eye = {0, 0, 2.0},
        .fov = 45.0f,
        .front = glm::vec3(0, 0, -1),
        .yaw = -90.0f,
        .pitch = 0.0f,
        .viewport = Viewport(0, 0, window.getWidth(), window.getHeight())
    });

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_d.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    GPUPipeline basicPipeline(shaderStages);
    

    // ================ Model Setup ================
    // set up data and vertex buffers
    initColourVertex();

    // ================ Rendering Mode Setup ================
    glPolygonMode(GL_FRONT, GL_FILL);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); 
    glEnable(GL_DEPTH_TEST);

    float lastFrameTime = glfwGetTime();

    // ================ Main Render and Event Loop ================
    while (! window.shouldClose()) {
        // 1. Poll events from the OS
        window.pollEvents();

        // 2. Update the frame clock/delta time
        float currentFrameTime = glfwGetTime();
        float dt = currentFrameTime - lastFrameTime;

        // 3. Run systems using window's delta time directly
        moveSys.tick(ecsWorld, objId, window.getInputState(), dt);
        camSys.tick(ecsWorld, window.getInputState(), dt);

        // 4. Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader
        // the order is always TRS (translate, rotate, scale) for the modelview matrix
        // the rotation is around the local axes (intrinsic) : x-axis first, then y-axis and z-axis
        TransformComp& trans = ecsWorld.getComp<TransformComp>(objId);
      
        basicPipeline.setMat4("uModel", trans.getLocalMatrix());

        TransformComp & transPlayer = ecsWorld.getComp<TransformComp>(player);
        CameraComp & camera = ecsWorld.getComp<CameraComp>(player);

        // set up the view matrix for the camera
        //glm::mat4 view = camera.getViewMatrix();
        //glm::mat4 proj = camera.getProjMatrix();

        // update the view matrix uniform in the shader
        basicPipeline.setMat4("uView",  camera.getViewMatrix());
        basicPipeline.setMat4("uProj", camera.getProjMatrix());

        drawColourVertex();


        window.swapBuffers();

        // set mouse x and y offsets to 0
        window.clearInputState();

        lastFrameTime = currentFrameTime;
    }


    return 0;
}