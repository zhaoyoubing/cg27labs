#pragma once

#include <glm/glm.hpp>


struct GPULight
{
    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 colour;
    glm::vec4 params;
};

static_assert(sizeof(GPULight) == 64);