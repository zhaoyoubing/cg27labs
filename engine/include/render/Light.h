#pragma once

#include <glm/glm.hpp>

enum LightType {
    DIRECTIONAL = 0,
    POINT = 1,
    SPOT = 2
};

// this Light struct is used to pass light information to the shader
// it is not intended for ECS
struct Light {
    LightType  type; // 0 = Directional, 1 = Point, 2 = Spot
    glm::vec3 position; // for point and spot lights
    glm::vec3 direction; // for directional and spot lights
    glm::vec3 color;
    float intensity;
    //float constant;
    //float linear;
    //float quadratic;
    float cutOff;
    float outerCutOff;
};