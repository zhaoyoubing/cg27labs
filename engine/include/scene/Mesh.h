#pragma once

#include "Vertex.h"
#include "AABB.h"

#include <glm/glm.hpp>

#include <vector>
#include <cstdint>

/*
struct Vertex {
    glm::vec3 pos;       // 12 bytes
    glm::vec3 normal;    // 12 bytes
    glm::vec2 uv;        // 8 bytes, texture coordinates
};
*/
class Mesh {
public:
    // positions, normals, uvs
    std::vector<Vertex> vertices;
    // triangle vertex indices
    std::vector<uint32_t> indices;

    // Additional vertex attribtues
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> colours;

    AABB bbox;

    Mesh() = default;

    void updateBBox() {
        // TODO
    }
};