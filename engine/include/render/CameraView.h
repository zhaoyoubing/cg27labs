// include/render/CameraView.h

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
//#include "entity/CameraComp.h"

enum class ProjectionType {
    Perspective,
    Orthographic
};

struct Viewport {
    float x = 0.0f;
    float y = 0.0f;
    float w = 800.0f;
    float h = 600.0f;

    float getAspectRatio() const {
        return (h > 0.0f) ? (w / h) : (16.0f / 9.0f);
    }
};
