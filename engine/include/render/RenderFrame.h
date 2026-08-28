#pragma once
#include <glm/glm.hpp>

struct RenderFrame {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
};