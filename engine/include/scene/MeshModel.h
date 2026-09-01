#pragma once

#include "scene/Mesh.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class MeshModel {
public:
    std::vector<std::shared_ptr<Mesh>> meshes;

};