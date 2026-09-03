#include <glad/glad.h>
#include <glm/glm.hpp>

#include "device/MeshBufferGPU.h"

#include <spdlog/spdlog.h>

#include <cstddef>
#include <utility>


MeshBufferGPU::~MeshBufferGPU()
{
    if (indexEBO_)
        glDeleteBuffers(1, &indexEBO_);

    if (colourVBO_)
        glDeleteBuffers(1, &colourVBO_);

    if (tangentVBO_)
        glDeleteBuffers(1, &tangentVBO_);

    if (vertexVBO_)
        glDeleteBuffers(1, &vertexVBO_);

    if (vao_)
        glDeleteVertexArrays(1, &vao_);
}

// create and upload buffers
void MeshBufferGPU::createBufferIds(std::shared_ptr<MeshGeometry> mesh)
{
    if ( mesh->vertices.empty() || mesh->indices.empty()) {
        spdlog::error("Mesh empty, quitting createBufferIds.");
        return;
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vertexVBO_);
    glGenBuffers(1, &indexEBO_);

    if (!mesh->tangents.empty())
        glGenBuffers(1, &tangentVBO_);
    
    if (!mesh->colours.empty())
        glGenBuffers(1, &colourVBO_);
}


// create and upload buffers
void MeshBufferGPU::uploadBuffers2GPU(std::shared_ptr<MeshGeometry> mesh)
{
    if ( ! vertexVBO_ || ! indexEBO_) {
        spdlog::error("Invalid VBO or EBO , quitting uploadBuffers.");
        return;
    }

    glBindVertexArray(vao_);

    // --------------------------------------------------
    // positions, normals, uvs
    // --------------------------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO_);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->vertices.size() * sizeof(Vertex),
        mesh->vertices.data(),
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, pos)));

    if (mesh->bNormal) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, normal)));
    }

    if (mesh->bTexture) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex),
            reinterpret_cast<void*>(offsetof(Vertex, uv)));
    }

    
    // --------------------------------------------------
    // Tangents
    // --------------------------------------------------

    if (tangentVBO_)
    {
        glBindBuffer(GL_ARRAY_BUFFER, tangentVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh->tangents.size() * sizeof(glm::vec3),
            mesh->tangents.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(3);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // Colours
    // --------------------------------------------------
    if (colourVBO_)
    {
        glBindBuffer(GL_ARRAY_BUFFER, colourVBO_);

        glBufferData(
            GL_ARRAY_BUFFER,
            mesh->colours.size() * sizeof(glm::vec3),
            mesh->colours.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(4);

        glVertexAttribPointer(
            4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    }

    // --------------------------------------------------
    // Indices
    // --------------------------------------------------
    glGenBuffers(1, &indexEBO_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO_);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh->indices.size() * sizeof(uint32_t),
        mesh->indices.data(),
        GL_STATIC_DRAW);

    
    indexCount_ = static_cast<GLsizei>(mesh->indices.size());

    spdlog::debug("Index count {}", indexCount_);
    

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

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, nullptr);
}

