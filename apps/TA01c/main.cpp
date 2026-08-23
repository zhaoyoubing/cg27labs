#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "render/Pipeline.h"
#include "drawColourVertex.h"

int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello OpenGL A01b", NULL, NULL);
    glfwMakeContextCurrent(window);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }


    // Load individual shader stages from disk
    Shader vertShader(ShaderStage::Vertex, "shaders/vcolour_c.vert");
    Shader fragShader(ShaderStage::Fragment, "shaders/vcolour.frag");
    
    // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
    std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
    Pipeline basicPipeline(shaderStages);
    
    // set up the view matrix for the camera
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.25f, 0.25f, -0.25f), // Camera position in world space
        glm::vec3(0.0f, 0.0f, 0.0f), // Look at the origin
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
    );

    glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

    // set up data and vertex buffers
    initColourVertex();

    // drawing mode and colour
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // setting the main render and event loop
    while (!glfwWindowShouldClose(window)) {
        // clear the background colour
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind the pipeline (shader program) for rendering
        basicPipeline.bind();

        // update the view matrix uniform in the shader
        basicPipeline.setMat4("matView", view);
        basicPipeline.setMat4("matProj", view);

        drawColourVertex();

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}