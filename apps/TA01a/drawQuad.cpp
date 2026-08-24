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

    // create the vertex buffer id
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void drawQuadRawTriangles() {
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

    // create vertex buffer
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawQuadIndexedTriangles() {
    // draw triangle using indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}