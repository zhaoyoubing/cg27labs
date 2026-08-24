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

#include "ui/InputState.h"
#include "ui/InputCallbacks.h"
#include "InputCallbacks.h"

ECSWorldPool gEcsWorld;

int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    // ================ GLFW and Glad Setup ================
    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello OpenGL A01c", NULL, NULL);
    glfwMakeContextCurrent(window);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ================ Init Transform Component ================
    EntityID objId = gEcsWorld.CreateEntityID();
    TransformComp trans;
    gEcsWorld.AddComp<TransformComp>(objId, trans);

    // ================ Keyboard and Mouse Setup ================
    InputCallbacks::Init(window);
    InputCallbacks::BindKeyCallback(key_callback);

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_c.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    Pipeline basicPipeline(shaderStages);
    
    /*
    // manually set up the position, orientation and scale of the object 
    glm::mat4 mat_scale = glm::scale(glm::vec3(1.2f, 1.2f, 1.2f));
    glm::mat4 mat_rot_x = glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 mat_rot_y = glm::rotate(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mat_trans = glm::translate(glm::vec3(0.2f, -0.3f, 0.0f));
    
    // set up the view matrix for the camera
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.f, 0.f, 5.f), // Camera position in world space
        glm::vec3(0.0f, 0.0f, 0.0f), // Look at the origin
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );

    //glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    */

    // ================ Model Setup ================
    // set up data and vertex buffers
    initColourVertex();

    // ================ Rendering Mode Setup ================
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
    glEnable(GL_DEPTH_TEST);

    // ================ Main Render and Event Loop ================
    while (!glfwWindowShouldClose(window)) {
        // clear the background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader
        // the order is always TRS (translate, rotate, scale) for the modelview matrix
        // for rotation we choose to rotate around the local x-axis first, then the y-axis
        TransformComp& trans = gEcsWorld.GetComp<TransformComp>(objId);
        glm::mat4 mat_model = glm::translate(trans.pos)  
            * glm::rotate(glm::radians(trans.rot.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::radians(trans.rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) 
            * glm::scale(glm::vec3(1.0f));
        
        basicPipeline.setMat4("matModel", mat_model);


        // update the view matrix uniform in the shader
        // basicPipeline.setMat4("matView", view);
        // basicPipeline.setMat4("matProj", proj);

        drawColourVertex();

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}