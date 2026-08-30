#pragma once

//#include "Vertex.h"
#include "AABB.h"

#include <glm/glm.hpp>

#include <vector>
#include <cstdint>

class Mesh {
public:
    // vertex attribtues
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> colours;

    // triangle vertex indices
    std::vector<uint32_t> indices;

    AABB bbox;

    Mesh() = default;

    void updateBBox() {
        // TODO
    }
};