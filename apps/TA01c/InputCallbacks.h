#pragma once

#include <glfw/glfw3.h>

#include "ecs/EcsWorldRegistry.h"

extern ECSWorldRegistry gEcsWorld;
extern TransformComp gTrans;

// the GLFW keyboard callback 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT ) {
            gTrans.rot.y -= 5.0;
        } else if (key == GLFW_KEY_RIGHT) {
            gTrans.rot.y += 5.0;
        } if (key == GLFW_KEY_DOWN ) {
            gTrans.rot.x += 5.0;
        } else if (key == GLFW_KEY_UP) {
            gTrans.rot.x -= 5.0;
        }

        if (key == GLFW_KEY_A ) {
            gTrans.pos.x -= 0.05;
        } else if (key == GLFW_KEY_D ) {
           gTrans.pos.x += 0.05;
        } if (key == GLFW_KEY_W ) {
           gTrans.pos.z += 0.05;
        } else if (key == GLFW_KEY_S) {
           gTrans.pos.z -= 0.05;
        } else if (key == GLFW_KEY_E) {
           gTrans.pos.y += 0.05;
        } else if (key == GLFW_KEY_Q) {
           gTrans.pos.y -= 0.05;
        }
    }
}