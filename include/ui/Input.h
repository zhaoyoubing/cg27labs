#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <functional>

class Input {
public:
    // --- Callback Signatures for Client Projects ---
    using KeyCallbackFn   = std::function<void(int key, int action)>;
    using MouseMoveFn     = std::function<void(double xpos, double ypos)>;
    using MouseButtonFn   = std::function<void(int button, int action)>;

    // --- Initialization ---
    static void Init(GLFWwindow* window) {
        // Register static GLFW callbacks
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, MousePosCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
    }

    static double GetMouseX() { return s_MouseX; }
    static double GetMouseY() { return s_MouseY; }

    // --- 2. Event Binding (For Client Projects / Custom Scripts) ---
    static void BindKeyCallback(KeyCallbackFn cb)         { s_KeyCallbacks.push_back(cb); }
    static void BindMouseMoveCallback(MouseMoveFn cb)     { s_MouseMoveCallbacks.push_back(cb); }
    static void BindMouseButtonCallback(MouseButtonFn cb) { s_MouseButtonCallbacks.push_back(cb); }

private:
    // --- Inline Static Storage (No instantiation needed) ---
    inline static std::unordered_map<int, bool> s_KeyStates;
    inline static std::unordered_map<int, bool> s_MouseButtonStates;
    
    inline static double s_MouseX = 0.0;
    inline static double s_MouseY = 0.0;

    inline static std::vector<KeyCallbackFn>    s_KeyCallbacks;
    inline static std::vector<MouseMoveFn>      s_MouseMoveCallbacks;
    inline static std::vector<MouseButtonFn>    s_MouseButtonCallbacks;

    // --- GLFW Trampoline Callbacks ---
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS)   s_KeyStates[key] = true;
        if (action == GLFW_RELEASE) s_KeyStates[key] = false;

        for (auto& cb : s_KeyCallbacks) {
            cb(key, action);
        }
    }

    static void MousePosCallback(GLFWwindow* window, double xpos, double ypos) {
        s_MouseX = xpos;
        s_MouseY = ypos;

        for (auto& cb : s_MouseMoveCallbacks) {
            cb(xpos, ypos);
        }
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS)   s_MouseButtonStates[button] = true;
        if (action == GLFW_RELEASE) s_MouseButtonStates[button] = false;

        for (auto& cb : s_MouseButtonCallbacks) {
            cb(button, action);
        }
    }
};