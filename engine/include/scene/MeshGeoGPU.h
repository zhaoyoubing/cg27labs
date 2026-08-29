// scene/MeshGeoGPU.h
#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Primitives.h"

// vao vbo and ebo of mesh geometry
class MeshGeoGPU {
    public:
    MeshGeoGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        setupGeometry(vertices, indices);
    }

    ~MeshGeoGPU();

    // Non-copyable, movable (RAII)
    MeshGeoGPU(const MeshGeoGPU&) = delete;
    MeshGeoGPU& operator=(const MeshGeoGPU&) = delete;
    MeshGeoGPU(MeshGeoGPU&&) noexcept;
    MeshGeoGPU& operator=(MeshGeoGPU&&) noexcept;

    void bindAndDraw() const;

private:
    unsigned int vao_ {0};
    unsigned int vbo_ {0};
    unsigned int ebo_ {0};
    unsigned int indexCount_ {0};

    void setupGeometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};