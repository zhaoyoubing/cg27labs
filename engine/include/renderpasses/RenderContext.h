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

};