#pragma once

#include <glad/glad.h> // Include if you use GLAD for OpenGL loading (or your preferred loader)
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window {
private:
    GLFWwindow* m_Window = nullptr;
    int m_Width;
    int m_Height;
    std::string m_Title;

    float m_LastFrameTime = 0.0f;
    float m_DeltaTime = 0.0f; // Tracked internally

public:
    Window(int width, int height, const std::string& title)
        : m_Width(width), m_Height(height), m_Title(title) {
        
        // 1. Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            exit(EXIT_FAILURE);
        }


#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE; // Required for macOS
#endif

        // 2. Create the window object
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // 3. Make the OpenGL context current
        glfwMakeContextCurrent(m_Window);

        // 4. Initialize GLAD (or your loader)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Set viewport size
        glViewport(0, 0, m_Width, m_Height);

        // Optional: Set user pointer so you can access the Window instance inside callbacks later
        glfwSetWindowUserPointer(m_Window, this);

        // Setup resize callback
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    }

    ~Window() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
        }
        glfwTerminate();
    }

    // Check if the window should close (ESC pressed or close button clicked)
    bool ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    // Poll OS window & input events (Must be called at the start of the frame)
    void PollEvents() const {
        glfwPollEvents();
    }

    // Swap the front and back rendering buffers
    void SwapBuffers() const {
        glfwSwapBuffers(m_Window);
    }

    // Returns current time in seconds since GLFW started
    float GetTime() const {
        return static_cast<float>(glfwGetTime());
    }

    // Call this once per frame, right after polling events
    void UpdateDeltaTime() {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrameTime - m_LastFrameTime;
        m_LastFrameTime = currentFrameTime;
    }

    float GetDeltaTime() const {
        return m_DeltaTime;
    }

    // Getters for window dimensions
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

    // Expose the raw GLFWwindow pointer if needed by input callbacks or external APIs
    GLFWwindow* GetNativeWindow() const { return m_Window; }
};