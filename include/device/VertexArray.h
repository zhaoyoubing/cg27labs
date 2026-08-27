#pragma once

#include <glad/glad.h>

class VertexArray {
public:
    unsigned int rendererID = 0;

    VertexArray() {
        glGenVertexArrays(1, &rendererID);
    }

    ~VertexArray() {
        if (rendererID) glDeleteVertexArrays(1, &rendererID);
    }

    // This method handles adding a separate stream to a specific shader location!
    void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, 
                         unsigned int layoutLocation, 
                         int componentCount, 
                         GLenum dataType, 
                         uint32_t stride, 
                         void* offset) 
    {
        bind();
        vertexBuffer->bind();

        glEnableVertexAttribArray(layoutLocation);
        glVertexAttribPointer(layoutLocation, componentCount, dataType, GL_FALSE, stride, offset);

        vertexBuffer->unbind();
        unbind();
    }

    void bind() const { glBindVertexArray(rendererID); }
    void unbind() const { glBindVertexArray(rendererID); } // (or 0)
};