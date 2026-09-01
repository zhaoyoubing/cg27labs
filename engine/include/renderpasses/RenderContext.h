#pragma once

#include "entity/EcsWorldRegistry.h"
#include "resources/MeshManager.h"
#include "resources/MaterialManager.h"
#include "resources/TextureManager.h"

struct RenderContext
{
    ECSWorldRegistry & registry_;

    MeshManager & meshMgr_;

    MaterialManager & matMgr_;

    TextureManager & texMgr_;

};