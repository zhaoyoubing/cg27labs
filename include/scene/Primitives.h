#pragma once

#include <glm/glm.hpp>

// Primitives for 3D computer graphics
// such as Vertex, Edge, Triangle

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};
