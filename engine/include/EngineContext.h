#pragma once

#include "render/TextureManager.h"

class EngineContext {
public:
    static TextureManager& getTextureManager() {
        static TextureManager instance;
        return instance;
    }
};