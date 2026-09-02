#pragma once

#include "device/MeshBufferGPU.h"

#include <vector>
using MeshHandle = uint32_t;

class MeshManager {
public:
    MeshHandle add(std::shared_ptr<MeshBufferGPU>  meshBuf) {
        meshBufList_.push_back(meshBuf);
        return meshBufList_.size() - 1;
    }

    std::shared_ptr<MeshBufferGPU> & get(MeshHandle handle) {
        return meshBufList_[handle];
    }

private:
   std::vector<std::shared_ptr<MeshBufferGPU> > meshBufList_;

};
