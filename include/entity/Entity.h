
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
// bitset representing the available components of an entity 
using Signature = std::bitset<MAX_COMPONENTS>;

// generate component type ids automatically
// relying on a function-local static variable typeId inside a template function
class ComponentIdCounter {
public:
    template<typename T>
    static std::size_t GetId() {
        static std::size_t typeId = s_Counter++;
        return typeId;
    }
private:
    inline static std::size_t s_Counter = 0;
};


// The entity component system pool/world
// More entities can be added to the pool as needed
// Example entity types: transform, camera, mesh, light; player, enemy, npc, etc.
class ECSWorldPool
{
private:

    EntityID nextId  = 0;                  // Tracks the next fresh ID to give out
    std::vector<EntityID> freeIdList;    // Pool of recycled IDs from destroyed entities
    
    // Entity ID -> Component Bitset Signature
    std::unordered_map<EntityID, Signature> signatures;

    // Component Type ID -> (Entity ID -> Component Data)
    // We use std::any or a type-erased wrapper to store different component structs in one map.
    std::unordered_map<std::size_t, std::unordered_map<EntityID, std::shared_ptr<void>>> compMaps;

public:

    EntityID CreateEntityID() {
        EntityID newId;
        
        // If we have recycled IDs from deleted entities, reuse them first
        if (!freeIdList.empty()) {
            newId = freeIdList.back();
            freeIdList.pop_back();
        } else {
            // Otherwise, hand out a brand new sequential ID
            newId = nextId++;
        }

        signatures[newId] = Signature{};
        return newId;
    }

    void DestroyEntityID(EntityID eid) {
        // 1. Check if the entity actually exists (has a signature)
        if (signatures.find(eid) == signatures.end()) {
            return; 
        }

        // 2. Remove the entity from all component pools/maps
        for (auto& [typeId, typeMap] : compMaps) {
            typeMap.erase(eid);
        }

        // 3. Remove its signature tracking
        signatures.erase(eid);

        freeIdList.push_back(eid);
    }


    template<typename T, typename... Args> 
    void AddComp(EntityID id, const T& component) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();

        // Directly emplace into the inner unordered_map for this component type
        auto& typeMap = compMaps[typeId];
        typeMap[id] = std::make_shared<T>(std::forward<Args>(args)...);
        
        // Flip the bit on for this component type in the entity's signature
        signatures[id].set(typeId, true);
    }

    template<typename T>
    void RemoveComp(EntityID id) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();
        
        if (compMaps.find(typeId) != compMaps.end()) {
            compMaps[typeId].erase(e);
        }

        m_Signatures[id].set(typeId, false);
    }

    template<typename T>
    T& GetComp(EntityID id) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();
        auto& ptr = compMaps[typeId][id];
        return *static_cast<T*>(ptr.get());
    }

    template <typename T>
    bool HasComp(EntityID id) const {
        // 1. Get the unique ID for this specific component type
        size_t compId = ComponentIdCounter::GetId<T>();

        // 2. Look up the entity's signature and check if that bit is true
        // Assuming entitySignatures is something like: std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>>
        
        auto it = signatures.find(id);
        if (it != signatures.end()) {
            return it->second.test(compId);
        }

        // Return false if the entity doesn't exist
        return false; 
    }


    // retrieve all entity IDs that have all the specified components
    // returning them as a vector
    template<typename... ComponentTypes> 
    std::vector<EntityID> View() {
        // Build a target bitset mask for the requested components
        Signature targetSignature;
        ((targetSignature.set(ComponentIdCounter::GetId<ComponentTypes>(), true)), ...);

        std::vector<EntityID> matchingEntities;
        for (const auto& [entity, signature] : signatures) {
            // Check if entity has ALL requested components using bitwise AND
            if ((signature & targetSignature) == targetSignature) {
                matchingEntities.push_back(entity);
            }
        }
        return matchingEntities;
    }


};
