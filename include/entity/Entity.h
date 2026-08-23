
#pragma once

#include <cstdint>
#include <unordered_map>

#include "TransformComp.h"

// A simple Entity type as a 32-bit unsigned integer. 
using EntityID = std::uint32_t;

// The entity component system pool/world
// More entities can be added to the pool as needed
// Example entity types: ransform, Camera, Mesh, Light; Player, Enemy, NPC, etc.
class ECSWorldPool
{
public:

    std::unordered_map<EntityID, TransformComp> transformPool;
    std::unordered_map<EntityID, TransformGPUComp> transformGPUPool;


    void setTransform(EntityID e, const TransformComp & t)  {
        transformPool[e] = t;
    }

    void setTransformGPU(EntityID e, const TransformGPUComp & t) {
        transformGPUPool[e] = t;
    }

    const TransformComp& getTransformStrict(EntityID e) const {
        return transformPool.at(e); // Throws exception if missing, returns read-only ref
    }

    const TransformGPUComp& getTransformGPUStrict(EntityID e) const {
        return transformGPUPool.at(e); // Throws exception if missing, returns read-only ref
    }
};