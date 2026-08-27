#pragma once

#include <glm/glm.hpp>

// Primitives for 3D computer graphics
// such as Vertex, Edge, Triangle

struct Vertex {
    glm::vec3 pos;       // 12 bytes
    glm::vec3 normal;    // 12 bytes
    glm::vec3 tagent;    // 12 bytes
    glm::vec2 uv;        // 8 bytes
    glm::vec2 uv1;       // 8 bytes
};
