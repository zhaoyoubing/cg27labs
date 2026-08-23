#pragma once

#include <glfw/glfw3.h>

#include "entity/Entity.h"

extern ECSWorldPool gEcsWorld;

// the GLFW API is 
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
void key_callback(int key, int action) {
    if (action == GLFW_PRESS) {
        // Assuming entity ID 1 for simplicity
        TransformComp& trans = gEcsWorld.transformPool[1]; 

        if (key == GLFW_KEY_LEFT ) {
            trans.rot.y -= 5.0;
        } else if (key == GLFW_KEY_RIGHT) {
            trans.rot.y += 5.0;
        } if (key == GLFW_KEY_DOWN ) {
            trans.rot.x += 5.0;
        } else if (key == GLFW_KEY_UP) {
            trans.rot.x -= 5.0;
        }

        if (key == GLFW_KEY_A ) {
            trans.pos.x -= 0.05;
        } else if (key == GLFW_KEY_D ) {
            trans.pos.x += 0.05;
        } if (key == GLFW_KEY_W ) {
            trans.pos.z += 0.05;
        } else if (key == GLFW_KEY_S) {
            trans.pos.z -= 0.05;
        } else if (key == GLFW_KEY_E) {
            trans.pos.y += 0.05;
        } else if (key == GLFW_KEY_Q) {
            trans.pos.y -= 0.05;
        }
    }
}