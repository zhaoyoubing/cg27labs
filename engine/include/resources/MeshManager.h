#pragma once

#include "device/MeshBufferGPU.h"

#include <vector>

class MeshManager {
public:
    MeshHandle add(std::shared_ptr<MeshBufferGPU>  mesh) {
        MeshHandle h {
            static_cast<std::uint32_t>(meshes_.size())
        };

        meshes_.push_back(mesh);

        return h;
    }

    std::shared_ptr<MeshBufferGPU> & get(MeshHandle h) {
        return meshes_[h];
    }

private:
   std::vector<std::shared_ptr<MeshBufferGPU> > meshes_;

};
