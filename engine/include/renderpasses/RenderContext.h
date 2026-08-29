#pragma once

#include "entity/EcsWorldRegistry.h"
#include "render/CameraView.h"

struct RenderContext
{
    ECSWorldRegistry & registry;

    CameraView & camera;
   
    int viewportWidth;
    int viewportHeight;

};