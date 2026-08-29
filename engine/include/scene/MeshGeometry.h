#pragma once

#include <vector>
#include "scene/Primitives.h"


class MeshGeometry {
public:
    // vertex list
    std::vector<Vertex> vertices;

    // triangle vertex indices
    std::vector< unsigned int > indices;

    bool bNormal_ = true;
    bool bTagent_ = false;
    bool bTex = true;
    bool bTex1 = false;

    MeshGeometry(const std::vector<Vertex>& vList, const std::vector<unsigned int>& idxList) {
        vertices = vList;
        indices = idxList;
    }
};