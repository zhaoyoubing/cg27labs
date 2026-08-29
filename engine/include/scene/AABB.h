#pragma once

#include "glm/glm.hpp"

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 getDimension() {
        return max - min;
    }

    glm::vec3 getCenter() {
        return (max + min) * 0.5f;
    }
};