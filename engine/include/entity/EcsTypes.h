
#pragma once

#include <cstdint>
#include <limits>
#include <unordered_map>
#include <bitset>

// A simple Entity type as a 32-bit unsigned integer. 
using EntityID = std::uint32_t;

constexpr EntityID INVALID_ENTITY = std::numeric_limits<EntityID>::max();
const EntityID MAX_ENTITIES = 5000;


const std::uint8_t MAX_COMPONENTS = 32;

// component mask for an entity
// bitset representing the available components of an entity 
using Signature = std::bitset<MAX_COMPONENTS>;

// generate component type ids automatically
// relying on a function-local static variable typeId inside a template function
class ComponentIdCounter {
public:
    template<typename T>
    static std::size_t GetId() {
        static std::size_t typeId = counter++;
        return typeId;
    }
private:
    inline static std::size_t counter = 0;
};

