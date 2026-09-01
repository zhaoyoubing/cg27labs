// engine/include/entity/CameraComp.h
#pragma once

#include "render/CameraView.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraComp {

    // Projection parameters
    ProjectionType projectionType = ProjectionType::Perspective;

    // camera position
    glm::vec3 eye = glm::vec3(0.0f);

    // perspective projection parameters
    float fov = 45.0f;   // Field of view in degrees (Perspective)
    float near = 0.1f;
    float far = 100.0f;
    //float aspect = 1.0f;
    
    float orthoSize = 5.0f;     // Half-height size (Orthographic)

    // the camera frame
    // Look orientation vectors & angles
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // rotation orientation
    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;

    // Viewport layout
    Viewport viewport;
    
    // Cached matrices (computed by the CameraSystem)
    glm::mat4 matView = glm::mat4(1.0f);
    glm::mat4 matProj = glm::mat4(1.0f);
    
    // Control flags
    bool isPrimary = true;      // Main rendering camera flag
    bool isDirty = true;        // Triggers matrix recalculation if modified

        // It can query the world directly when needed
    glm::mat4 getProjMatrix() {
        if (isDirty) {
             updateMatrices(); // Recalculate matrices if dirty
        }
        
        return matProj;
    }

    // Calculates the view matrix using the entity's world position and camera front vector
    glm::mat4 getViewMatrix() {
        if (isDirty) {
            updateMatrices(); // Recalculate matrices if dirty
        }
        return matView;
    }

    void updateMatrices() {
        matProj = glm::perspective(glm::radians(fov), viewport.getAspectRatio(), near, far);
        matView = glm::lookAt(eye, eye + front, up);
        isDirty = false; // Reset dirty flag after recalculation
    }

};