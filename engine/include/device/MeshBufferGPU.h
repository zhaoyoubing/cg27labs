// device/MeshGeoGPU.h
#pragma once

#include <glad/glad.h>

#include "scene/MeshGeometry.h"

#include <glm/glm.hpp>

#include <memory>
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
    MeshBufferGPU(MeshBufferGPU&& other) noexcept = default;
    MeshBufferGPU& operator=(MeshBufferGPU&& other) noexcept = default;

    void createAndUploadBuffers(std::shared_ptr<MeshGeometry> mesh) {
        createBufferIds(mesh);
        uploadBuffers2GPU(mesh);
    }

    void bind() const;
    void draw() const;
    void unbind() const;

private:
    GLuint vao_ {0};

    GLuint vertexVBO_ {0};
    GLuint tangentVBO_ {0};
    GLuint colourVBO_ {0};

    GLuint indexEBO_ {0};

    GLsizei indexCount_ {0};

    void createBufferIds(std::shared_ptr<MeshGeometry>);
    void uploadBuffers2GPU(std::shared_ptr<MeshGeometry>);
};