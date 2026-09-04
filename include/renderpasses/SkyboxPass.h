#pragma once

#include "device/GPUPipeline.h
#include "RenderPass.h"

#include "memory"

class SkyboxPass : RenderPass
{
public:
    void render(
        const Skybox& skybox,
        const Camera& camera);

private:
    std::shared_ptr<GPUPipeline> shader;
};