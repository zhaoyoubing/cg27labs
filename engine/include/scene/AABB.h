#pragma once

#include <algorithm>
#include "glm/glm.hpp"

// Axis-Aligned Bounding Box (AABB) structure for 3D space, 
// defined by minimum and maximum corner points.
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    bool bValid = false;

    AABB() {
        min = glm::vec3(0.0f);
        max = glm::vec3(0.0f);
    }

    glm::vec3 getDimension() const { 
        return max - min;
    }

    glm::vec3 getCenter() const {
        return (max + min) * 0.5f;
    }

    float getMaxDimLen() const {
        glm::vec3 dim = getDimension();
        return std::max(dim.x, std::max(dim.y, dim.z));
    }

    AABB unite(const AABB & other) const {
        AABB result;

        if (bValid && other.bValid) {
            result.min = glm::min(min, other.min);
            result.max = glm::max(max, other.max);
            result.bValid = true;
        } else if (other.bValid) {
            result.min = other.min;
            result.max = other.max;
            result.bValid = true;
        }

        return result;
    }

    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }

    AABB intersect(const AABB& other) const {
        AABB result;
        result.min = glm::max(min, other.min);
        result.max = glm::min(max, other.max);
        return result;
    }
};