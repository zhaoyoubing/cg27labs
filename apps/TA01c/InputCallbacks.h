#pragma once

#include <glfw/glfw3.h>

#include "entity/TransformComp.h"

extern TransformComp gTrans;

// the GLFW keyboard callback 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT ) {
            // [TODO] tc2.1 
            // subtract gTrans.rot.y by 5.0 on the LEFT arrow key
            
        } else if (key == GLFW_KEY_RIGHT) {
            gTrans.rot.y += 5.0;
        } if (key == GLFW_KEY_DOWN ) {
            gTrans.rot.x += 5.0;
        } else if (key == GLFW_KEY_UP) {
            gTrans.rot.x -= 5.0;
        }

        if (key == GLFW_KEY_A ) {
            // [TODO] tc2.2 
            // subtract gTrans.pos.x by 0.05 when press A

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