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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    // adding a second attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3 * 4));

    // create index buffer
    GLuint idxBufID;
    glGenBuffers(1, &idxBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBufID);

    // set buffer data for triangle index
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void drawColourVertex() {
    // draw triangle using indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}