// scene/MeshGeoGPU.h
#pragma once

#include <glad/glad.h>

#include "Mesh.h"

#include <glm/glm.hpp>
#include <vector>

// vao vbo and ebo of mesh geometry
class MeshBufferGPU {
    public:
    MeshBufferGPU() { }

    ~MeshBufferGPU();

    // Non-copyable
    MeshBufferGPU(const MeshBufferGPU&) = delete;
    MeshBufferGPU& operator=(const MeshBufferGPU&) = delete;

    // Movable
    MeshBufferGPU(MeshBufferGPU&& other) noexcept;
    MeshBufferGPU& operator=(MeshBufferGPU&& other) noexcept;

    void bind() const;
    void draw() const;
    void unbind() const;

private:
    GLuint vao_ {0};

    GLuint positionVBO_ {0};
    GLuint normalVBO_ {0};
    GLuint uvVBO_ {0};
    GLuint tangentVBO_ {0};
    GLuint colourVBO_ {0};

    GLuint indexEBO_ {0};

    GLsizei indexCount_ {0};

    void genBuffers(Mesh & mesh);
};