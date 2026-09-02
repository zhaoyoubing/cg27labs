#pragma once

#include "entity/EcsWorldRegistry.h"
#include "scene/AABB.h"
#include "scene/SceneNode.h"

#include <string>
#include <memory>

struct Scene {
    std::string name_;
    ECSWorldRegistry ecsWorld_;

    Scene(const std::string& name = "default") : name_ (name) {}
    ~Scene() = default;

    //void init();
    //void update(float deltaTime);
    AABB updateBBox();

    std::unique_ptr<SceneNode> root_;
    //ECSWorldRegistry & getRegistry() { return registry_; }
};