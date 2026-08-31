#pragma once

#include "scene/Mesh.h"
#include <vector>

class MeshManager {
public:
    MeshID add(Mesh mesh) {
        MeshID id {
            static_cast<std::uint32_t>(meshes_.size())
        };

        meshes_.push_back(std::move(mesh));

        return id;
    }

    Meshe & get(MeshID id) {
        return meshes_[id];
    }

private:
    std::vector<Material> meshes_;

};
