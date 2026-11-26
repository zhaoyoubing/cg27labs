// engine/include/renderpasses/StdDeferredLightPass.h
#pragma once

#include "device/GPUPipeline.h"
#include "RenderPass.h"
#include "RenderContext.h"
#include "resources/MaterialManager.h"
#include "scene/SceneNode.h"

#include <memory>

#include "entity/CameraComp.h"
#include "entity/LightComp.h"

#include "device/GPULight.h"

// Deferred Light Pass with Shadow Mapping
class StdDeferredLightPass : public RenderPass
{
public:
    explicit StdDeferredLightPass() { }
    
    bool init(RenderContext& context) override;

    // Called once per frame.
    void execute(RenderContext& context) override;

    const int MAX_LIGHTS = 128; // not used at the moment

private:

    CameraComp camera_;
    LightComp lightDefault_;

    // framebuffer dimensions
    int width_ = 0;
    int height_ = 0;

    GLuint FBO = 0;
    GLuint texFBO = 0; // output framebuffer texture

    // for uploading light data
    // GLuint lightSSBO = 0;
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    //void setSSBOLights(const std::vector<GPULight> & lights);
    //void setPointLights(RenderContext & ctx);
};