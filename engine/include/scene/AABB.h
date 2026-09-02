#pragma once

#include "glm/glm.hpp"

// Axis-Aligned Bounding Box (AABB) structure for 3D space, 
// defined by minimum and maximum corner points.
struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 getDimension() const { 
        return max - min;
    }

    glm::vec3 getCenter() const {
        return (max + min) * 0.5f;
    }

    AABB unite(const AABB & other) const {
        AABB result;
        result.min = glm::min(min, other.min);
        result.max = glm::max(max, other.max);
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