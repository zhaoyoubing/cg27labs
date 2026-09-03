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

    bool bBoxDirty = true;
    
    // Optional: Only present if this node actually draws something
    std::vector<std::shared_ptr<MaterialMesh> > meshList; 

    std::vector<std::unique_ptr<SceneNode>> children;

    void updateBBox() {
        AABB box;

        for (auto & mesh : meshList) {
            box = box.unite(mesh->geometry_->getBBox());
        }

        for (auto & node : children) {
            box = box.unite(node->getBBox());
        }

        bbox = box;
    }

    AABB getBBox() {
        if (bBoxDirty) {
            updateBBox();
            bBoxDirty = false;
        }

        return bbox;
    }

    std::unique_ptr<SceneNode> clone() const {
        auto result = std::make_unique<SceneNode>();

        result->trans = trans;
        result->meshList = meshList;

        for (const auto& child : children)
        {
            result->children.push_back(child->clone());
        }

        return result;
    }
};