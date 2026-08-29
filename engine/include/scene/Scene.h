#pragma once

#include "entity/EcsWorldRegistry.h"

#include <string>

class Scene {
public:
    Scene(const std::string& name);
    ~Scene() = default;

    void init();
    void update(float deltaTime);

    ECSWorldRegistry & getRegistry() { return registry_; }

private:
    std::string name_;
    ECSWorldRegistry registry_;
};