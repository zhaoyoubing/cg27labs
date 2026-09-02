#pragma once

#include "entity/EcsWorldRegistry.h"
#include "resources/MeshManager.h"
#include "resources/MaterialManager.h"
#include "resources/TextureManager.h"
#include "resources/ShaderManager.h"
#include "scene/Scene.h"

#include <memory>

struct RenderContext
{
    //std::shared_ptr<Node> rootNode_; // Scene graph root for hierarchical scene representation

    std::shared_ptr<Scene> scene_; // Scene graph root for hierarchical scene representation

    //ECSWorldRegistry & registry_;

    MeshManager & meshMgr_;

    MaterialManager & matMgr_;

    TextureManager & texMgr_;

    ShaderManager & shaderMgr_;

};