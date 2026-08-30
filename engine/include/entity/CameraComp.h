// engine/include/entity/CameraComp.h
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraComp {

    // Projection parameters
    float fov = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    float aspect = 1.0f;
    
    // camera position
    glm::vec3 eye = glm::vec3(0.0f);

    // Look orientation vectors & angles
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // rotation orientation
    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;

};