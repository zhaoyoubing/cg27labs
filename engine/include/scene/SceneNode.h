#pragma once

#include "entity/TransformComp.h"
#include "scene/MaterialMesh.h"
#include "scene/AABB.h"

#include <string>
#include <vector>
#include <memory>

// A scene graph node, which can have a local transform, an optional mesh, and children nodes.
struct SceneNode {
    std::string name;
    TransformComp trans;  // local transform relative to parent
    AABB bbox;
    
    // Optional: Only present if this node actually draws something
    std::shared_ptr<MaterialMesh> renderable; 

    std::vector<std::unique_ptr<SceneNode>> children;

    std::unique_ptr<SceneNode> clone() const {
        auto result = std::make_unique<SceneNode>();

        result->trans = trans;
        result->renderable = renderable;

        for (const auto& child : children)
        {
            result->children.push_back(child->clone());
        }

        return result;
    }
};