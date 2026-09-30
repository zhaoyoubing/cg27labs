#include "drawColourVertex.h"

void initColourVertex() {
    GLfloat verts[] = {
        -0.5f, 0.5f, 0.f,   // v0
        1.f, 0.f, 0.f,      // Red
        -0.5f, -0.5f, 0.f,  // v1
        0.f, 1.f, 0.f,      // Green
        0.5f, -0.5f, 0.f,   // v2
        0.f, 0.f, 1.f,      // Blue
        0.5f, 0.5f, 0.f,    // v3
        1.f, 1.f, 0.f,      // Yellow
    };

        // indices of two triangles
    GLuint indices[] = { 0, 1, 2, 2, 3, 0};

    // create vertex buffer
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    
    // [TODO] Tb1 specifiy the vertex position attribute 
    // using glVertexAttribPointer(...)
    
    // [TODO] Tb2 adding a second attribute using glEnableVertexAttribArray(1)
    // and specifiy the colour attribute using glVertexAttribPointer(...)

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawColourVertex() {
    // [TODO] Tb3 draw indexed triangle using glDrawElements(GL_TRIANGLES, ...)

}