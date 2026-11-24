#pragma once

#include <glm/glm.hpp>


struct GPULight
{
    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 colour;
    glm::vec4 params;
};

/*
// 64 bytes total (4 x 16-byte aligned rows) — match layout identically on CPU (C++)
struct GpuLight {
    float3 position;      // 12 bytes (xyz) — used by Point & Spot (ignored for Directional)
    float  range;         //  4 bytes (w)   — max influence radius (or 1/range^2 precomputed)

    float3 direction;     // 12 bytes (xyz) — unit direction (Spot axis or Directional ray dir)
    float  cosOuterCone;  //  4 bytes (w)   — cos(outerAngle) for Spot lights

    float3 color;         // 12 bytes (rgb) — linear RGB radiance/color
    float  intensity;     //  4 bytes (w)   — scalar intensity / luminous multiplier

    uint   type;          //  4 bytes (x)   — 0: Directional, 1: Point, 2: Spot
    float  cosInnerCone;  //  4 bytes (y)   — cos(innerAngle) for Spot falloff
    int    shadowIndex;   //  4 bytes (z)   — index into shadow/cascade table (-1 if unshadowed)
    float  volumetricMul; //  4 bytes (w)   — optional fog/froxel contribution scale
};
*/
static_assert(sizeof(GPULight) == 64);