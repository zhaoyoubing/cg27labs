#pragma once

#include "Vertex.h"
#include "AABB.h"

#include <vector>

class MeshGeometry {
public:
    // vertex list
    std::vector<Vertex> vertices;

    // triangle vertex indices
    std::vector< unsigned int > indices;

    AABB bbox;

    bool bNormal_ = true;
    bool bTagent_ = false;
    bool bTex = true;
    bool bTex1 = false;

    MeshGeometry() = default;

    MeshGeometry(const std::vector<Vertex>& vList, const std::vector<unsigned int>& idxList) {
        vertices = vList;
        indices = idxList;
    }

    void updateBBox() {
        // TODO
    }
};