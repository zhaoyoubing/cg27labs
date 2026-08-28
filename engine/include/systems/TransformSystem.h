// Pseudo-code representation of your ECS registry/world
#include "entity/TransformComp.h"
class TransformSystem {
public:
    void updateTransforms(Registry& registry) {
        // Step 1: Find all "Root" entities (entities that have a Transform, but NO Parent)
        auto roots = registry.view<TransformComp, GlobalTransformComp>()
                            .exclude<ParentComponent>();

        // Step 2: Recursively calculate matrices down the hierarchy tree
        for (auto entity : roots) {
            calculateGlobalMatrixRecursive(entity, glm::mat4(1.0f), registry);
        }
    }

private:
    void calculateGlobalMatrixRecursive(uint32_t entityID, const glm::mat4& parentMatrix, Registry& registry) {
        // Get components for this entity
        auto& transform = registry.get<TransformComp>(entityID);
        auto& globalTransform = registry.get<GlobalTransformComp>(entityID);

        // 1. Calculate local matrix
        glm::mat4 localMatrix = transform.getLocalMatrix();

        // 2. Multiply with parent's global matrix to get this entity's global matrix
        globalTransform.worldMatrix = parentMatrix * localMatrix;

        // 3. If this entity has children, recurse downwards
        if (registry.has<ChildComp>(entityID)) {
            const auto& children = registry.get<ChildComponent>(entityID).childrenEntityIDs;
            for (uint32_t childID : children) {
                // Pass THIS entity's newly computed world matrix down as the parent matrix
                calculateGlobalMatrixRecursive(childID, globalTransform.worldMatrix, registry);
            }
        }
    }
};