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
    
    // 3. Global Settings / Uniform Buffer Data
    //struct Settings {
    float exposure = 1.0f;
    float bloomIntensity = 1.5f;
    float bloomThreshold = 0.8f;
    int  blurPassCount = 4;
    int downSampleFactor = 4;
    //} settings;
    
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

};