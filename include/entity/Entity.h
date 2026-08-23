
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

};