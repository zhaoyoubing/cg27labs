// engine/include/entity/TransformComp.h
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// Transform Component, contains ONLY DATA
// A simple component to hold the position, rotation, and scale of a Transform in 3D space. This component can be used to represent the transformation of an entity in a scene graph or a game engine.
struct TransformComp {
    glm::vec3 pos = {0.f, 0.f, 0.f};;
    glm::vec3 rot = {0.f, 0.f, 0.f};;
    glm::vec3 scale = {1.f, 1.f, 1.f};

    // using intrinsic rotation in an order of X-Y-Z, or Pitch-Yaw-Roll
    glm::mat4 getLocalMatrix() const {
         return glm::translate(pos)  
            * glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) 
            * glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 0.0f, 1.0f)) 
            * glm::scale(scale);
    }
};

// Transform Matrix Component for GPU
struct TransformGPUComp {
    glm::mat4 matModel;
    // glm::mat4 matNormal;  // calculate normal matrix on CPU for better efficiency
};
