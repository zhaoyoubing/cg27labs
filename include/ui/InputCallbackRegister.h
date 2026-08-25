#pragma once

#include <iostream>

#include "InputState.h"

class InputCallbackRegister {

public:
    static void init(GLFWwindow* window, InputState * input) {
        // Store a pointer to InputState
        glfwSetWindowUserPointer(window, input);

        // Register GLFW callbacks
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
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
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            input->keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            input->keyStates[key] = false;
        }
    }

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        // std::cout << "mouse move" << std::endl;

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

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        //std::cout << "mouse button" << std::endl;

        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            input->mouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            input->mouseButtonStates[button] = false;
        }
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        input->scrollX = xoffset;
        input->scrollY = yoffset;
    }
};