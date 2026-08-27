// scene/MeshGPU.h
#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Primitives.h"

class MeshGPU {
    public:
    MeshGPU() = default;
    MeshGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        setupGeometry(vertices, indices);
    }
    
    ~MeshGPU();

    // Non-copyable, movable (RAII)
    MeshGPU(const MeshGPU&) = delete;
    MeshGPU& operator=(const MeshGPU&) = delete;
    MeshGPU(MeshGPU&&) noexcept;
    MeshGPU& operator=(MeshGPU&&) noexcept;

    void bindAndDraw() const;

private:
    unsigned int vao_ {0};
    unsigned int vbo_ {0};
    unsigned int ebo_ {0};
    unsigned int indexCount_ {0};

    void setupGeometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};