#pragma once

#include <memory>
#include <glad/glad.h>

#include "device/GPUPipeline.h"
struct PipelineState {
    std::shared_ptr<GPUPipeline> gPipe;
    bool depthTest = true;
    bool depthWrite = true;
    bool colorWrite = true;
    GLenum cullMode = GL_BACK;
    // Blend mode, topology, etc. can go here
};

