#pragma once

#include <vector>
#include <unordered_map>
#include <bitset>
#include <cstdint>


#include "ecs/EcsTypes.h"

// Assuming types are defined globally:
// using EntityID = std::uint32_t;
// using Signature = std::bitset<32>;

class EntityIdAllocator {
private:
    EntityID nextId = 0;
    std::vector<EntityID> freeIdList;
    std::unordered_map<EntityID, Signature> signatures;

public:
    EntityID createEntity() {
        EntityID newId;
        if (!freeIdList.empty()) {
            newId = freeIdList.back();
            freeIdList.pop_back();
        } else {
            newId = nextId++;
        }
        signatures[newId] = Signature{};
        return newId;
    }

    void destroyEntity(EntityID eid) {
        if (signatures.find(eid) == signatures.end()) {
            return; 
        }
        signatures.erase(eid);
        freeIdList.push_back(eid);
    }

    bool isValid(EntityID eid) const {
        return signatures.find(eid) != signatures.end();
    }

    Signature& getSignature(EntityID eid) {
        return signatures[eid];
    }

    const Signature& getSignature(EntityID eid) const {
        return signatures.at(eid);
    }

    const std::unordered_map<EntityID, Signature>& getAllSignatures() const {
        return signatures;
    }
};