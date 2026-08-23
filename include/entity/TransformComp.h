#pragma once

#include <glm/glm.hpp>

// Transform Component, contains ONLY DATA
// A simple component to hold the position, rotation, and scale of a Transform in 3D space. This component can be used to represent the transformation of an entity in a scene graph or a game engine.
struct TransformComp {
    glm::vec3 pos = {0.f, 0.f, 0.f};;
    glm::vec3 rot = {0.f, 0.f, 0.f};;
    glm::vec3 scale = {1.f, 1.f, 1.f};
};

// Transform Matrix Component for GPU
struct TransformGPUComp {
    glm::mat4 matModel;
    // glm::mat4 matNormal;  // calculate normal matrix on CPU for better efficiency
};
