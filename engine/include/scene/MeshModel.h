#pragma once

#include "scene/MeshGeometry.h"
#include "scene/MeshNode.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

// We still use a SceneGraph structure for the model for its intuitive hierarchical 
// representation of meshes and their transforms, 
// but we can also accommodate that to ECS, where each MeshNode is an entity with a MeshComp and TransformComp
class MeshModel {
public:
    std::vector<std::shared_ptr<MeshNode>> meshNodes;

    void Draw() const {
        // Traverse the node hierarchy to render
        for (const auto& node : rootNodes) {
            DrawNode(node, glm::mat4(1.0f), shader);
        }
    }
private:
    void DrawMeshNode(const std::shared_ptr<MeshNode>& node, const glm::mat4& parentMatrix) const {
        glm::mat4 globalTransform = parentMatrix * node->transform;

        for (int meshIdx : node->meshIndices) {
            const auto& meshNode = mesheNodes[meshIdx];
            // Bind material textures, set uniforms, and draw VAO...
            meshNode->draw();
        }

        for (const auto& child : node->children) {
            DrawMeshNode(child, globalTransform);
        }
    }
};