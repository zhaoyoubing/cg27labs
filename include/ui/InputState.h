#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

class InputState {
public:
    static void Init(GLFWwindow* window) {
        // Store a pointer to our singleton or manager instance
        glfwSetWindowUserPointer(window, &GetInstance());

        // Register GLFW callbacks
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, ScrollCallback);
    }

    // Call this at the very beginning or end of your frame loop 
    // to reset transient states (like single-frame clicks/scrolls)
    static void PollEvents() {
        glfwPollEvents();
    }

    static bool IsKeyHeld(int key) {
        auto& inst = GetInstance();
        auto it = inst.m_KeyStates.find(key);
        return it != inst.m_KeyStates.end() ? it->second : false;
    }

    // ============ Mouse Event Handling ============
    // Call this at the end of your main loop frame to reset deltas & scroll
    static void NewFrame() {
        auto& inst = GetInstance();
        inst.m_MouseOffsetX = 0.0;
        inst.m_MouseOffsetY = 0.0;
        inst.m_ScrollX = 0.0;
        inst.m_ScrollY = 0.0;
    }

    static double GetMouseX() { return GetInstance().m_MouseX; }
    static double GetMouseY() { return GetInstance().m_MouseY; }
    static double GetMouseOffsetX() { return GetInstance().m_MouseOffsetX; }
    static double GetMouseOffsetY() { return GetInstance().m_MouseOffsetY; }

private:
    InputState() = default;
    
    // Meyer's Singleton pattern
    static InputState & GetInstance() {
        static InputState instance;
        return instance;
    }

    // --- GLFW Callback Trampolines ---
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            input->m_KeyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            input->m_KeyStates[key] = false;
        }
    }

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        input->m_MouseX = xpos;
        input->m_MouseY = ypos;
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            input->m_MouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            input->m_MouseButtonStates[button] = false;
        }
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* input = static_cast<InputState*>(glfwGetWindowUserPointer(window));
        input->m_ScrollX = xoffset;
        input->m_ScrollY = yoffset;
    }

    std::unordered_map<int, bool> m_KeyStates;
    std::unordered_map<int, bool> m_MouseButtonStates;
    
    double m_MouseX = 0.0, m_MouseY = 0.0;
    double m_LastMouseX = 0.0, m_LastMouseY = 0.0;
    double m_MouseOffsetX = 0.0, m_MouseOffsetY = 0.0;
    double m_ScrollX = 0.0, m_ScrollY = 0.0;
    bool bFirstMouse = true;
};