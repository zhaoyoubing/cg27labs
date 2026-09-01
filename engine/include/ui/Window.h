#pragma once

#include "inputState.h"
#include "InputCallbackRegister.h"

#include <glad/glad.h> // Include if you use GLAD for OpenGL loading (or your preferred loader)
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>


class Window {
private:
    GLFWwindow* window_ = nullptr;
    int width_;
    int height_;
    std::string title_;

    InputState inputState_;

public:
    Window(int width, int height, const std::string& title)
        : width_(width), height_(height), title_(title) {
        
        // 1. Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            exit(EXIT_FAILURE);
        }


#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // Required for macOS
#endif

        // 2. Create the window object
        window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
        if (!window_) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // 3. Make the OpenGL context current
        glfwMakeContextCurrent(window_);

        // 4. Initialize GLAD (or your loader)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            exit(EXIT_FAILURE);
        }

        InputCallbackRegister::init(window_, &inputState_);

        // Set viewport size
        glViewport(0, 0, width_, height_);

        // Optional: Set user pointer so you can access the Window instance inside callbacks later
        // glfwSetWindowUserPointer(window_, this);

        // Setup resize callback
        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    }

    ~Window() {
        if (window_) {
            glfwDestroyWindow(window_);
        }
        glfwTerminate();
        
    }

    // Check if the window should close (ESC pressed or close button clicked)
    bool shouldClose() const {
        return glfwWindowShouldClose(window_);
    }

    // Poll OS window & input events (Must be called at the start of the frame)
    void pollEvents() const {
        glfwPollEvents();
    }

    // Swap the front and back rendering buffers
    void swapBuffers() const {
        glfwSwapBuffers(window_);
    }
    
    void show() const {
        glfwShowWindow(window_);
    }

    InputState & getInputState() { return inputState_; }

    // Getters for window dimensions
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    // Expose the raw GLFWwindow pointer if needed by input callbacks or external APIs
    GLFWwindow* getNativeWindow() const { return window_; }

    void clearInputState() { inputState_.clearOffset(); }

    void clearScreen() { 
        // 1. Specify the RGBA color to use when clearing (e.g., a dark slate gray background)
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);

        // 2. Clear both the Color Buffer (what you see) and the Depth Buffer (for 3D mesh rendering)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


};