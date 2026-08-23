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
        0.0f, 0.0f, -0.5f,    // v4
        0.f, 1.f, 1.f,      // Cyan
    };

    // indices of six triangles
    GLuint indices[] = { 
        // the original two triangles form the base
        0, 1, 2, 2, 3, 0,
        // four new triangles
        4, 0, 1,
        4, 1, 2,
        4, 2, 3,
        4, 3, 0
    };

    // create vertex buffer
    GLuint vertBufID;
    glGenBuffers(1, &vertBufID);
    glBindBuffer(GL_ARRAY_BUFFER, vertBufID);

    // set buffer data to triangle vertex and setting vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    // adding a second attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawColourVertex() {
    // draw triangle using indices
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}