#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
 
    std::cout << "Hello, Graphics!" << std::endl; 

    // GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL A01", NULL, NULL);
    glfwMakeContextCurrent(window);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // setting the main rendering and event loop
    while (!glfwWindowShouldClose(window)) {
        // render
        glClear(GL_COLOR_BUFFER_BIT);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}