#pragma once

#include <glm/glm.hpp>

// Primitives for 3D computer graphics
// such as Vertex, Edge, Triangle

struct Vertex {
    glm::vec3 pos;       // 12 bytes
    glm::vec3 normal;    // 12 bytes
    glm::vec2 uv;        // 8 bytes, texture coordinates
    glm::vec3 tangent;   // 12 bytes, for normal mapping
};
