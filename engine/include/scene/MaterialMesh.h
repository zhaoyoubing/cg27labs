#pragma once

#include "MeshGeometry.h"
#include "render/Material.h"

#include <glm/glm.hpp>
#include <memory>

class MaterialMesh {
public:
    std::shared_ptr<MeshGeometry> geometry_;
    std::shared_ptr<Material> material_;

    //int materialIndex = -1; //   Index into the material array for this mesh node

    MaterialMesh() = default;
    MaterialMesh(std::shared_ptr<MeshGeometry> mesh) : geometry_(mesh) {}

    void createAndUploadBuffers() {
        if (geometry_) {
            //mesh_->createAndUploadBuffers();
        }
    }

    void draw() const {
        if (geometry_) {
            geometry_->draw();
        }
    }

};
