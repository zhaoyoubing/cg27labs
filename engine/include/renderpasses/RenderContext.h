#pragma once

#include "entity/EcsWorldRegistry.h"
#include <render/CameraView.h>

struct RenderContext
{
    EcsWorldRegistry & registry;

    CameraView & camera;
    RenderResources& resources;
    //int viewportWidth;
    //int viewportHeight;

};