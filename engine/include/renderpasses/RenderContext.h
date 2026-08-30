#pragma once

#include "entity/EcsWorldRegistry.h"
#include "render/CameraView.h"
#include "render/MaterialManager.h"
#include "render/TextureManager.h"

struct RenderContext
{
    ECSWorldRegistry & registry_;

    CameraView & camera_;

    MaterialManager & matMgr_;
    
    TextureManager & texMgr_;

    //int viewportWidth;
    //int viewportHeight;

};