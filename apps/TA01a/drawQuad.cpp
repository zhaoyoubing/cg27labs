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

    // [TODO] Ta1.1 
    GLuint vertBufId;
    // create the vertex buffer id using glGenBuffers(1, address of your_buffer_Id)
    
    // and bind the vertex buffer using glBindBuffer(GL_ARRAY_BUFFER, your_buffer_Id)


    // [TODO] Ta1.2 
    // set the vertex buffer data to verts using glBufferData(GL_ARRAY_BUFFER,...)
    
    // enable vertex attributes 0 (position) using glEnableVertexAttribArray(0)
    
    // specify vertex attribute 0 layout using  glVertexAttribPointer(...)
 

}

void drawQuadRawTriangles() {
    // [TODO] Ta1.3 
    // draw raw triangles using glDrawArrays(GL_TRIANGLES, ...)

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

    GLuint vertBufId;
    // [TODO] Ta3.1 create the vertex buffer id using glGenBuffers(...)
    // and bind the vertex buffer using glBindBuffer(GL_ARRAY_BUFFER, ...)


    // [TODO] Ta3.2 set the vertex buffer datta to verts using glBufferData(GL_ARRAY_BUFFER, ...)
    // specify vertex attributes using glEnableVertexAttribArray(0)
    // and glVertexAttribPointer(...)


    GLuint idxBufId;
    // [TODO] Ta3.3 create and bind the index buffer using 
    // glGenBuffers and glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ...)

    // [TODO] Ta3.4 set the triangle index buffer using
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, ...)


}

void drawQuadIndexedTriangles() {
    // [TODO] Ta3.5 draw indexed triangle using glDrawElements(GL_TRIANGLES, ...)

}