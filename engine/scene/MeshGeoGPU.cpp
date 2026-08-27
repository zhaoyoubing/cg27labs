#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <utility>

#include "scene/MeshGeoGPU.h"


MeshGeoGPU::~MeshGeoGPU()
{
    if (ebo_ != 0)
        glDeleteBuffers(1, &ebo_);

    if (vbo_ != 0)
        glDeleteBuffers(1, &vbo_);

    if (vao_ != 0)
        glDeleteVertexArrays(1, &vao_);
}

MeshGeoGPU::MeshGeoGPU(MeshGeoGPU&& other) noexcept
    : vao_(other.vao_),
      vbo_(other.vbo_),
      ebo_(other.ebo_),
      indexCount_(other.indexCount_)
{
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ebo_ = 0;
    other.indexCount_ = 0;
}

MeshGeoGPU& MeshGeoGPU::operator=(MeshGeoGPU&& other) noexcept
{
    if (this == &other)
        return *this;

    // Release current resources
    if (ebo_ != 0)
        glDeleteBuffers(1, &ebo_);

    if (vbo_ != 0)
        glDeleteBuffers(1, &vbo_);

    if (vao_ != 0)
        glDeleteVertexArrays(1, &vao_);

    // Take ownership
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ebo_ = other.ebo_;
    indexCount_ = other.indexCount_;

    // Empty the source
    other.vao_ = 0;
    other.vbo_ = 0;
    other.ebo_ = 0;
    other.indexCount_ = 0;

    return *this;
}

void MeshGeoGPU::setupGeometry(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices)
{
    indexCount_ = static_cast<unsigned int>(indices.size());

    // Create OpenGL objects
    glCreateVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ebo_);

    // Upload vertex data
    glNamedBufferData(
        vbo_,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // Upload index data
    glNamedBufferData(
        ebo_,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(),
        GL_STATIC_DRAW
    );

    // ------------------------------------------------------------
    // Vertex buffer binding
    // ------------------------------------------------------------

    constexpr GLuint vertexBinding = 0;

    glVertexArrayVertexBuffer(
        vao_,
        vertexBinding,
        vbo_,
        0,
        sizeof(Vertex)
    );

    // ------------------------------------------------------------
    // Position - location 0
    // ------------------------------------------------------------

    glEnableVertexArrayAttrib(vao_, 0);

    glVertexArrayAttribFormat(
        vao_,
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, position)
    );

    glVertexArrayAttribBinding(
        vao_,
        0,
        vertexBinding
    );

    // ------------------------------------------------------------
    // Normal - location 1
    // ------------------------------------------------------------

    glEnableVertexArrayAttrib(vao_, 1);

    glVertexArrayAttribFormat(
        vao_,
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, normal)
    );

    glVertexArrayAttribBinding(
        vao_,
        1,
        vertexBinding
    );

    // ------------------------------------------------------------
    // Texture coordinates - location 2
    // ------------------------------------------------------------

    glEnableVertexArrayAttrib(vao_, 2);

    glVertexArrayAttribFormat(
        vao_,
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        offsetof(Vertex, texCoord)
    );

    glVertexArrayAttribBinding(
        vao_,
        2,
        vertexBinding
    );

    // ------------------------------------------------------------
    // Index buffer
    // ------------------------------------------------------------

    glVertexArrayElementBuffer(vao_, ebo_);
}

void MeshGeoGPU::bindAndDraw() const
{
    if (vao_ == 0 || indexCount_ == 0)
        return;

    glBindVertexArray(vao_);

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indexCount_),
        GL_UNSIGNED_INT,
        nullptr
    );

    glBindVertexArray(0);
}