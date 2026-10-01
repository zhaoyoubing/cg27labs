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
    // [TODO] Tc3.1 register function key_callback in glfw
    // using glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback)

    // ================ Shaders and Pipeline Setup ================
    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_c.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    GPUPipeline basicPipeline(shaderStages);

    // manually update the model matrix uniform in the shader
    // the order is always TRS (translate, rotate, scale) for the modelview matrix
    // for rotation we choose to rotate around the local x-axis first, then the y-axis
    // glm::mat4 mat_model = gTrans.getLocalMatrix();
    glm::mat4 mat_scale = glm::scale(glm::vec3(0.8f, 0.8f, 0.8f));
    glm::mat4 mat_rot_x = glm::rotate(glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 mat_rot_z = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 mat_trans = glm::translate(glm::vec3(0.2f, -0.5f, -0.2f));

    // ================ Model Setup ================
    // set up data and vertex buffers
    initColourVertex();

    // ================ Rendering Mode Setup ================
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    // [TODO] Tc1.2 enable depth testing using glEnable(GL_DEPTH_TEST)



    // ================ Main Render and Event Loop ================
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // clear the background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the model matrix uniform in the shader 
        // glm::mat4 mat_model = mat_trans * mat_rot_x * mat_rot_z * mat_scale;   

        // [TODO] tc3.2 set mat_model to gTrans.getLocalMatrix() in the following
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