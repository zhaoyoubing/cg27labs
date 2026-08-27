#pragma once

#include <vector>
#include "scene/Primitives.h"


class MeshGeometry {
public:
     std::vector<Vertex> vertices;

    // triangle vertex indices
    std::vector< unsigned int > indices;

    MeshGeometry() {

    }
};