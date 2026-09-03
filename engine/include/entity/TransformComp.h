// engine/include/entity/TransformComp.h
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

enum class TransformType {
    EULER,
    QUATERNION,
    MATRIX
};
// Transform Component, contains ONLY DATA
// A simple component to hold the position, rotation, and scale of a Transform in 3D space. This component can be used to represent the transformation of an entity in a scene graph or a game engine.
struct TransformComp {
    TransformType type = TransformType::EULER;

    // T
    glm::vec3 pos = {0.f, 0.f, 0.f};
    // R
    glm::vec3 rot = {0.f, 0.f, 0.f};
    glm::quat quat = glm::quat(1.0f, 0.f, 0.f, 0.f);
    // S
    glm::vec3 scale = {1.f, 1.f, 1.f};

    bool bMatrixDirty = true;
    glm::mat4 mat = glm::mat4(1.0);

    // using intrinsic rotation in an order of X-Y-Z, or Pitch-Yaw-Roll
    glm::mat4 getLocalMatrix() {
        if (type == TransformType::MATRIX)
            return mat;
        else if (bMatrixDirty) {
            glm::mat4 T = glm::translate(pos);
            glm::mat4 S = glm::scale(scale);

            glm::mat4 R = glm::mat4(1.0f);
            
            if (type == TransformType::QUATERNION)
               R = glm::mat4_cast(quat);
            else if (type == TransformType::EULER)
               R = glm::rotate(glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f))
                    * glm::rotate(glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) 
                    * glm::rotate(glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
            
            mat = T * R * S;

            bMatrixDirty = false;

            return mat;
        }
        
        return mat;    
    }
};

// Transform Matrix Component for GPU
/*
struct TransformGPUComp {
    glm::mat4 matModel;
    // glm::mat4 matNormal;  // calculate normal matrix on CPU for better efficiency
};
*/
