#pragma once

#include "entity/EcsWorldRegistry.h"
#include "scene/AABB.h"
#include "scene/SceneNode.h"
#include "entity/LightComp.h"

#include <string>
#include <memory>

struct Scene {
    std::string name_;
    ECSWorldRegistry ecsWorld_;

    std::vector<LightComp> lightList;

    Scene(const std::string& name = "default") : name_ (name) {
       root_ = std::make_unique<SceneNode>(); 
    }
    
    ~Scene() = default;

    //void init();
    //void update(float deltaTime);
    AABB getBBox() {
        return root_->getBBox();
    }

    std::unique_ptr<SceneNode> root_;
    //ECSWorldRegistry & getRegistry() { return registry_; }
};