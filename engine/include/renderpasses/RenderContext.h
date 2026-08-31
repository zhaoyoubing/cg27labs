#pragma once

#include "entity/EcsWorldRegistry.h"
#include "render/CameraView.h"
#include "resources/MeshManager.h"
#include "resources/MaterialManager.h"
#include "resources/TextureManager.h"

struct RenderContext
{
    ECSWorldRegistry & registry_;

    CameraView & camera_;

    MeshManager & meshMgr_;

    MaterialManager & matMgr_;

    TextureManager & texMgr_;

    //int viewportWidth;
    //int viewportHeight;

};