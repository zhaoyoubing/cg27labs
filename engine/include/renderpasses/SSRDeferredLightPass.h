// engine/include/renderpasses/SSRDeferredLightPass.h
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

// SSR offscreen Lighting Pass
class SSRDeferredLightPass : public RenderPass
{
public:
    explicit SSRDeferredLightPass() { }
    
    // called once before the rendering loop
    bool init(RenderContext& context) override;

    // Render function called once per frame.
    void execute(RenderContext& context) override;

    const int MAX_LIGHTS = 128;

private:

    CameraComp camera_;
    
    // input GBuffers
    GLuint gAlbedo = 0;
    GLuint gNormal = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;

    // framebuffer dimensions
    int width_ = 0;
    int height_ = 0;

    GLuint FBO = 0;
    // output framebuffer texture
    GLuint texFBO = 0;

    // for uploading light data
    std::shared_ptr<GPUPipeline> gpuPipe = nullptr;

    // void setSSBOLights(const std::vector<GPULight> & lights);
    void setPointLights(RenderContext & ctx);
};