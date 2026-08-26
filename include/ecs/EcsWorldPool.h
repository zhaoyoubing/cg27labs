#pragma once

#include "ecs/EcsTypes.h"

// The entity component system pool/world
// More entities can be added to the pool as needed
// Example entity types: transform, camera, mesh, light; player, enemy, npc, etc.
#pragma once

#include "ecs/entity/EntityIdRegistry.h"
#include <memory>
#include <unordered_map>
#include <vector>

class ECSWorldPool {
private:
    EntityIdRegistry entityRegistry;

    // Component Type ID -> (Entity ID -> Component Data)
    std::unordered_map<std::size_t, std::unordered_map<EntityID, std::shared_ptr<void>>> compMaps;

public:

    EntityID createEntityID() {
        return entityRegistry.createEntity();
    }

    void destroyEntityID(EntityID eid) {
        if (!entityRegistry.isValid(eid)) {
            return;
        }

        // 1. Remove the entity from all component pools/maps
        for (auto& [typeId, typeMap] : compMaps) {
            typeMap.erase(eid);
        }

        // 2. Let the EntityIdRegistry wipe signature and recycle the ID
        entityRegistry.destroyEntity(eid);
    }

    template<typename T, typename... Args> 
    void addComp(EntityID id, Args&&... args) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();

        auto& typeMap = compMaps[typeId];
        typeMap[id] = std::make_shared<T>(std::forward<Args>(args)...);
        
        // Update signature via EntityIdRegistry
        entityRegistry.getSignature(id).set(typeId, true);
    }

    template<typename T>
    void removeComp(EntityID id) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();
        
        if (compMaps.find(typeId) != compMaps.end()) {
            compMaps[typeId].erase(id);
        }

        entityRegistry.getSignature(id).set(typeId, false);
    }

    template<typename T>
    T& getComp(EntityID id) {
        std::size_t typeId = ComponentIdCounter::GetId<T>();
        auto& ptr = compMaps[typeId][id];
        return *static_cast<T*>(ptr.get());
    }

    template <typename T>
    bool hasComp(EntityID id) const {
        size_t compId = ComponentIdCounter::GetId<T>();
        if (entityRegistry.isValid(id)) {
            return entityRegistry.getSignature(id).test(compId);
        }
        return false; 
    }

    template<typename... ComponentTypes> 
    std::vector<EntityID> view() {
        Signature targetSignature;
        ((targetSignature.set(ComponentIdCounter::GetId<ComponentTypes>(), true)), ...);

        std::vector<EntityID> matchingEntities;
        for (const auto& [entity, signature] : entityRegistry.getAllSignatures()) {
            if ((signature & targetSignature) == targetSignature) {
                matchingEntities.push_back(entity);
            }
        }
        return matchingEntities;
    }
};