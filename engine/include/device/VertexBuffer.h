#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    unsigned int rendererID = 0;

    // Constructor: automatically allocates and uploads data
    template<typename T>
    VertexBuffer(const std::vector<T>& data) {
        glGenBuffers(1, &rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, rendererID);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    }

    ~VertexBuffer() {
        if (rendererID) glDeleteBuffers(1, &rendererID);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, rendererID); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};