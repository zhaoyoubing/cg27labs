#pragma once

#include "entity/EcsWorldRegistry.h"
#include "resources/MeshManager.h"
#include "resources/MaterialManager.h"
#include "resources/TextureManager.h"
#include "resources/ShaderManager.h"
#include "scene/Scene.h"

#include <memory>

// for shadow mapping
struct ShadowData {
    GLuint depthMap = 0;
    glm::mat4 matLightSpace{1.0f};
};

struct GBufferData {
    //GLuint gBufFBO = 0;
    GLuint gAlbedo = 0;
    GLuint gNormal = 0;
    GLuint gMat = 0;   // Metallic-Roughness
    GLuint gDepth = 0;
    glm::mat4 matInvProj{1.0f};
};

struct PostProcessData {
    // 1. Resources (Transient RT pool)
    GLuint texScene;  // should be HDR in bloom
    GLuint bloomBufferA;
    GLuint bloomBufferB;
    
    // 2. Global Settings / Uniform Buffer Data
    float exposure = 1.0f;
    float bloomIntensity = 1.5f;
    float bloomThreshold = 0.8f;
    int  blurPassCount = 4;
    int downSampleFactor = 4;
};

struct SSAOData {
    // --- Quality & Kernel ---
    int sampleCount = 64;          // Number of hemisphere samples (e.g., 32 or 64)
    float radius = 0.5f;           // World-space sampling radius (how far occluders look)
    float bias = 0.025f;           // Depth bias to prevent self-shadowing/shadow acne
    
    // --- Visual Tuning ---
    float intensity = 1.5f;        // Multiplier to darken or soften the occlusion contrast
    float power = 1.0f;            // Exponent for contrast falloff (optional)
    
    // --- Performance / Resolution ---
    bool halfResolution = false;   // Render SSAO at half-res for performance, then upscale/blur
};

struct RenderContext
{
    std::shared_ptr<Scene> scene_; // Scene graph root for hierarchical scene representation

    MeshManager & meshMgr_;

    MaterialManager & matMgr_;

    TextureManager & texMgr_;

    ShaderManager & shaderMgr_;

    ShadowData shadow_;

    GBufferData gbuf_;

    PostProcessData postp_;

    SSAOData ssao_;

};