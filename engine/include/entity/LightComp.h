// engine/include/entity/LightComp.h

#pragma once

#include <glm/glm.hpp>

enum LightType {
    DIRECTIONAL = 0,
    POINT = 1,
    SPOT = 2
};

// this Light struct is used to pass light information to the shader
// For teaching purpose, we till put position and dir in the Light stucture
// Ideally, it should be separated to be put in a TransformComp

struct LightComp {
    LightType type; // 0 = Directional, 1 = Point, 2 = Spot
    glm::vec3 pos; // for point and spot lights
    glm::vec3 dir; // for directional and spot lights
    glm::vec3 colour;
    float intensity;

    // attenuation a + b x + c x^2
    float a; // constant
    float b; // linear
    float c; // quadratic

    float cutOff;   // for spot lights
    float outerCutOff;  // for spot lights
};