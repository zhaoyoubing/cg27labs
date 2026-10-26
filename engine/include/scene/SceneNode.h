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
    //bool bMeshRoot = false;

    AABB bbox;
    bool bBoxDirty = true;
    
    // Optional: Only present if this node actually draws something
    std::vector<std::shared_ptr<MaterialMesh> > meshList; 

    std::vector<std::unique_ptr<SceneNode>> children;

    // scale to fit for a model's root node
    void scaleToFitBBox(float d) {
        updateBBox();
        float len = bbox.getMaxDimLen();
        float scale = d / len;
        trans.scale = glm::vec3(scale, scale, scale);

        trans.bMatrixDirty = true;
        //bBoxDirty = true;
    }

    void center() {
        glm::vec center = bbox.getCenter();
        trans.pos = glm::vec3(- center.x * trans.scale.x,
                            - center.y * trans.scale.y,
                            - center.z * trans.scale.z );
        trans.bMatrixDirty = true;
    }


    // need to take transform into account
    void updateBBox() {

        AABB box;

        for (auto & mesh : meshList) {
            //box = box.unite(mesh->geometry_->getBBox(mat * trans.getLocalMatrix()));   
            box = box.unite(mesh->geometry_->getBBox());           
        }

        for (auto & node : children) {
            //box = box.unite(node->getBBox(mat * trans.getLocalMatrix()));
            box = box.unite(node->getBBox());
        }

        bbox = box;
    }

    AABB getBBox() {
        if (bBoxDirty) {
            updateBBox();
            bBoxDirty = false;
        }

        // need to apply matrix
        AABB box;
        box.min = glm::vec3(trans.getLocalMatrix() * glm::vec4(bbox.min, 1.0));
        box.max = glm::vec3(trans.getLocalMatrix() * glm::vec4(bbox.max, 1.0));
        box.bValid = bbox.bValid;

        return box;
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