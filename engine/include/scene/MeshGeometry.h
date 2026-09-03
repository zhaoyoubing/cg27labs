#pragma once

#include "Vertex.h"
#include "AABB.h"
//#include "device/MeshBufferGPU.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <cstdint>

/*
struct Vertex {
    glm::vec3 pos;       // 12 bytes
    glm::vec3 normal;    // 12 bytes
    glm::vec2 uv;        // 8 bytes, texture coordinates
};
*/

using MeshHandle = uint32_t;

class MeshBufferGPU;

// The triangular mesh geometry representation of a 3D object
// using vertices, indices, and additional attributes like tangents and colors. 
// It also includes an axis-aligned bounding box (AABB) for rendering and spatial queries.

class MeshGeometry {
public:
    // positions, normals, uvs
    std::vector<Vertex> vertices;
    // triangle vertex indices
    std::vector<uint32_t> indices;

    // Additional vertex attribtues
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> colours;

    bool bNormal = true;
    bool bTexture = true;

    AABB bbox;

    bool bBoxDirty = true;

    // VAO, VBO, EBO handles for GPU rendering
    std::shared_ptr<MeshBufferGPU> gpuBuffer; // GPU buffer for rendering

    MeshGeometry() = default;
    MeshGeometry(std::vector<Vertex> vList, std::vector<uint32_t> idxList)
        : vertices(std::move(vList)), indices(std::move(idxList)) {

    }

    AABB getBBox() {
        if (bBoxDirty) {
            updateBBox();
            bBoxDirty = false;
        }
        
        return bbox;
    }

    void updateBBox() {
        if (vertices.empty()) {
            bbox.min = glm::vec3(0.0f);
            bbox.max = glm::vec3(0.0f);
        }

        bbox.min = vertices[0].pos;
        bbox.max = vertices[0].pos;

        for (const auto& vertex : vertices) {
            bbox.min = glm::min(bbox.min, vertex.pos);
            bbox.max = glm::max(bbox.max, vertex.pos);
        }
    }
};