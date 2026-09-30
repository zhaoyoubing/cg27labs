#include "drawQuad.h"

// draw two triangles using raw vertex data (triangle soup)
void initQuadRawTriangles() {
    GLfloat verts[] = {
        -0.5f, 0.5f, 0.f,   // v0
        -0.5f, -0.5f, 0.f,  // v1
        0.5f, -0.5f, 0.f,   // v2

        0.5f, -0.5f, 0.f,  // v2
        0.5f, 0.5f, 0.f,   // v3
        -0.5f, 0.5f, 0.f,  // v0
    };

    // T1a create the vertex buffer id using glGenBuffers(...)
    // and bind the vertex buffer using glBindBuffer(...)


    // T1b set the vertex buffer datta to verts using glBufferData(...)
    // specify vertex attributes using glEnableVertexAttribArray(0)
    // and glVertexAttribPointer(...)
 

}

void drawQuadRawTriangles() {
    // T1c draw raw triangles using glDrawArrays(GL_TRIANGLES, ...)

}

// draw two triangles using indexed drawing
void initQuadIndexedTriangles() {
    GLfloat verts[] = {
        -0.5f, 0.5f, 0.f,   // v0
        -0.5f, -0.5f, 0.f,  // v1
        0.5f, -0.5f, 0.f,   // v2
        0.5f, 0.5f, 0.f,   // v3
    };

    // indices of two triangles
    GLuint indices[] = { 0, 1, 2, 2, 3, 0};

    // T2a create the vertex buffer id using glGenBuffers(...)
    // and bind the vertex buffer using glBindBuffer(GL_ARRAY_BUFFER, ...)


    // T2b set the vertex buffer datta to verts using glBufferData(GL_ARRAY_BUFFER, ...)
    // specify vertex attributes using glEnableVertexAttribArray(0)
    // and glVertexAttribPointer(...)


    // T2c create and bind the index buffer using 
    // glGenBuffers and glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ...)

    // T2d set the triangle index buffer using
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, ...)


}

void drawQuadIndexedTriangles() {
    // T2e draw indexed triangle using glDrawElements(GL_TRIANGLES, ...)
    
}