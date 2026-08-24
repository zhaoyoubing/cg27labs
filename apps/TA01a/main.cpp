#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "drawQuad.h"

int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello OpenGL A01a", NULL, NULL);
    glfwMakeContextCurrent(window);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set up data and vertex buffers
    // initQuadRawTriangles();
    initQuadIndexedTriangles();

    // drawing mode and colour
    // wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // filled polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // red colour
    glColor3f(1.0f, 0.0f, 0.0f);

    // setting the main render and event loop
    while (!glfwWindowShouldClose(window)) {
        // clear the background colour
        glClear(GL_COLOR_BUFFER_BIT);

        // real drawing of the quad
        // drawQuadRawTriangles();
        drawQuadIndexedTriangles();

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}