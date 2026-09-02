// include/entity/MeshComp.h
#pragma once

#include <memory>

#include "device/MeshBufferGPU.h"


// the renderable mesh
struct MeshComp {
    std::shared_ptr<MeshBufferGPU> meshBuf;
};