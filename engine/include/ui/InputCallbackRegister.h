#pragma once

#include "InputState.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output

#include <iostream>

class InputCallbackRegister {

public:
    static void init(GLFWwindow* window, InputState * input) {
        // Store a pointer to InputState
        glfwSetWindowUserPointer(window, input);

        // Register GLFW callbacks
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetScrollCallback(window, scrollCallback);
    }

private:
    InputCallbackRegister() = default;
    
    // Meyer's Singleton pattern
    /*
    static InputCallbackRegister & GetInstance() {
        static InputCallbackRegister instance;
        return instance;
    }
    */

    // --- GLFW Callback Trampolines ---
    // GLFW callbacks must be static
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            input->keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            input->keyStates[key] = false;
        }
    }

    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        spdlog::trace("Mouse move event: x = {}, y ={}", xpos, ypos);

        // first time mouse data
        if (input->bFirstMouse) {
            input->lastMouseX = xpos;
            input->lastMouseY = ypos;
            input->bFirstMouse = false;
        } else {
            input->lastMouseX = input->mouseX;
            input->lastMouseY = input->mouseY;
        }

        input->mouseX = xpos;
        input->mouseY = ypos;

        input->mouseOffsetX += input->mouseX - input->lastMouseX;
        input->mouseOffsetY += input->lastMouseY - input->mouseY; // Reversed: y-coordinates go bottom-to-top

    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        
        spdlog::trace("Mouse button event : {}", button);

        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            input->mouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            input->mouseButtonStates[button] = false;
        }
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        input->scrollX = xoffset;
        input->scrollY = yoffset;
    }
};