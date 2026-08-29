#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

// InputState is a static class
// InputState instance is window associated
class InputState {
public:
    InputState() = default;
    
    bool isKeyHeld(int key) {
        auto it = keyStates.find(key);
        return it != keyStates.end() ? it->second : false;
    }

    // ============ Mouse Event Handling ========d====
    // Call this at the end of your main loop frame to reset deltas & scroll
    void clearOffset() {
        mouseOffsetX = 0.0;
        mouseOffsetY = 0.0;
        scrollX = 0.0;
        scrollY = 0.0;
    }

    double getMouseX() { return mouseX; }
    double getMouseY() { return mouseY; }
    double getMouseOffsetX() { return mouseOffsetX; }
    double getMouseOffsetY() { return mouseOffsetY; }

    bool isLeftButtonPressed() { return mouseButtonStates[GLFW_MOUSE_BUTTON_LEFT]; }
    bool isRightButtonPressed() { return mouseButtonStates[GLFW_MOUSE_BUTTON_RIGHT]; }
    bool isMiddleButtonPressed() { return mouseButtonStates[GLFW_MOUSE_BUTTON_MIDDLE]; }

    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    
    double mouseX = 0.0, mouseY = 0.0;
    double lastMouseX = 0.0, lastMouseY = 0.0;
    double mouseOffsetX = 0.0, mouseOffsetY = 0.0;
    double scrollX = 0.0, scrollY = 0.0;
    bool bFirstMouse = true;
};