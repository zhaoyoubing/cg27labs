#include "drawColourVertex.h"
#include "InputCallbacks.h"

#include "device/GPUPipeline.h"
#include "entity/TransformComp.h"

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

TransformComp gTrans;

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

    // ================ Keyboard Setup ================
    // [TODO] Tc1 register function key_callback in glfw
    // using glfwSetKeyCallback(...)

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_c.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    GPUPipeline basicPipeline(shaderStages);


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
        glfwPollEvents();

        // clear the background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader 
        // [TODO] tc2 set mat_model to gTrans.getLocalMatrix() in the following
        glm::mat4 mat_model = glm::mat4(1.0);   

        basicPipeline.setMat4("uModel", mat_model);

        drawColourVertex();

        // swap buffers
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}