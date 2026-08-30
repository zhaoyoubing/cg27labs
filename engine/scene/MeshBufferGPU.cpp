#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <utility>

#include "scene/MeshBufferGPU.h"


MeshBufferGPU::~MeshBufferGPU()
{
    if (indexEBO_)
        glDeleteBuffers(1, &indexEBO_);

    if (colourVBO_)
        glDeleteBuffers(1, &colourVBO_);

    if (tangentVBO_)
        glDeleteBuffers(1, &tangentVBO_);

    if (uvVBO_)
        glDeleteBuffers(1, &uvVBO_);

    if (normalVBO_)
        glDeleteBuffers(1, &normalVBO_);

    if (positionVBO_)
        glDeleteBuffers(1, &positionVBO_);

    if (vao_)
        glDeleteVertexArrays(1, &vao_);
}


void MeshBufferGPU::genBuffers(Mesh & mesh)
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // --------------------------------------------------
    // Positions
    // --------------------------------------------------

    if (!mesh.positions.empty())
    {
        glGenBuffers(1, &positionVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, positionVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.positions.size() * sizeof(glm::vec3),
            mesh.positions.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // Normals
    // --------------------------------------------------

    if (!mesh.normals.empty())
    {
        glGenBuffers(1, &normalVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, normalVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.normals.size() * sizeof(glm::vec3),
            mesh.normals.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // UVs
    // --------------------------------------------------

    if (!mesh.uvs.empty())
    {
        glGenBuffers(1, &uvVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, uvVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.uvs.size() * sizeof(glm::vec2),
            mesh.uvs.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
    }

    // --------------------------------------------------
    // Tangents
    // --------------------------------------------------

    if (!mesh.tangents.empty())
    {
        glGenBuffers(1, &tangentVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, tangentVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.tangents.size() * sizeof(glm::vec3),
            mesh.tangents.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(3);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // Colours
    // --------------------------------------------------

    if (!mesh.colours.empty())
    {
        glGenBuffers(1, &colourVBO_);

        glBindBuffer(GL_ARRAY_BUFFER, colourVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh.colours.size() * sizeof(glm::vec3),
            mesh.colours.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(4);

        glVertexAttribPointer(
            4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // Indices
    // --------------------------------------------------

    if (!mesh.indices.empty())
    {
        glGenBuffers(1, &indexEBO_);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO_);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            mesh.indices.size() * sizeof(uint32_t),
            mesh.indices.data(),
            GL_STATIC_DRAW);

        indexCount_ = static_cast<GLsizei>(mesh.indices.size());
    }

    // The EBO binding is stored in the VAO.
    // Unbind the VAO afterwards.
    glBindVertexArray(0);
}

void MeshBufferGPU::bind() const
{
    glBindVertexArray(vao_);
}

void MeshBufferGPU::unbind() const
{
    glBindVertexArray(0);
}

void MeshBufferGPU::draw() const
{
    if (vao_ == 0 || indexCount_ == 0)
        return;

    //glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, nullptr);

    //glBindVertexArray(0);
}